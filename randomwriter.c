#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "hashset.h"
#include "vector.h"

bool debug = false;
//Structure to storage inside of the vectors
struct frequency{
    char ch;		// a particular letter
    int occurrences;	// the number of times it occurs
};

//hashset seeds is a hashset which elements are: 
struct hashelement {
	char *key;
	vector *value;
	char *letters;
};

void printFrequencies(void *elem,void *fp){
  struct frequency *freq = (struct frequency *)elem;
  fprintf((FILE *)fp, "%c-> %4d|", freq->ch, freq->occurrences);
} 

void printHashElems(void *elem,void *fp){
	struct hashelement *h = (struct hashelement *)elem;
	fprintf((FILE *)fp,"\nseed %s: ",h->key);
	VectorMap(h->value, printFrequencies, stdout);	

}

void calculateTotalLetters(void *elem,void *tL){
	int *totalLetters = (int *)tL;
	struct frequency *freq = (struct frequency *)elem;
	*totalLetters +=  freq->occurrences;
}

void insertLetters(void *elem,void *letters){
	int i;
	char **s = (char **)letters;
	struct frequency *freq = (struct frequency *)elem;
	for(i=0;i<freq->occurrences;i++){
		*(*s) = freq->ch; 
		(*s)++;
	}

}

void genLetters(void *elem,void *ad){
	int totalLetters = 0;
	char *p;
	struct hashelement *h = (struct hashelement *)elem;
	VectorMap(h->value, calculateTotalLetters, (void *)&totalLetters);	
	h->letters = (char *)malloc(sizeof(char) * (totalLetters + 1));
	p = h->letters;
	VectorMap(h->value, insertLetters, (void *)&p);	
	*p = '\0';
	if(debug) printf("%s letters: %s \n",h->key,h->letters);
}

int randInt(int max){
	double scale =  1.0 / ((double) RAND_MAX + 1.0);
	return (int)(rand() * scale * (max -  1));
}

void getHashKeys(void *elem,void *vkeys){
	static int pos = 0;
	char **keys = (void *)vkeys;
	struct hashelement *h = (struct hashelement *)elem;
	keys[pos] = (char *)malloc(sizeof(char)*strlen(h->key));
	strcpy(keys[pos],h->key);
	if(debug) printf("key : |%s|",h->key);
	pos++;
}

void printKeys(void *elem,void *aux){
	char *key = (char *)elem;
	printf("|%s|",key);
}

void genrandomWords(hashset *seeds,int numwords){
	int totalWords = 0;
	srand(time(NULL));
	int numSeeds = HashSetCount(seeds);
	int seed = randInt(numSeeds);
	char currentSeed[100];
	char letter;
	int leng;
	struct  hashelement localHE;
	struct hashelement *found;
	char ** keys;
	char *space = " ";

	//For the moment
	//numwords = 10;
	keys = (char **)malloc(sizeof(char*) *numSeeds);
	//gen a vector of all the keys

	HashSetMap(seeds,getHashKeys,(void *)keys);
	

	//print the starter point
	strcpy(currentSeed,keys[seed]);
	printf("%s",currentSeed);
	localHE.key = currentSeed;
	while(totalWords <= numwords){
		//Search for the next letter
		found = (struct hashelement *) HashSetLookup(seeds,&localHE);	
		if (found != NULL){ //print the letter

			leng = strlen(found->letters);
			seed = randInt(leng);
			letter = found->letters[seed];
			if(debug) printf("<seed: |%s| letters: |%s| random: |%d| >\n",currentSeed,found->letters,seed);
			printf("%c",letter);
			if(letter == ' ') totalWords++;
			memmove(currentSeed,&currentSeed[1],strlen(currentSeed) - 1);
			currentSeed[strlen(currentSeed) - 1] = letter; 
			if(debug) printf("<seed: |%s| letter: |%c| >\n",currentSeed, letter);	 
		}else{
			//put a new seed
			seed = randInt(numSeeds);
			strcpy(currentSeed,keys[seed]);
			printf("%s",currentSeed);
			if(strstr(currentSeed,space) != NULL) totalWords++;
		}
		
	}
}

void usage(char *name){
	printf("use: %s k f w\n"
		"where: \n"
			"\tk : number of letters to group\n"
			"\tf : input file\n"
			"\tw : number of words to print\n" ,
		name);
}
//take the input file and split it on k letters
//char *output has the letters or leng of ouput is set 0 if no more letters
void slplitkletters(FILE *fd,int k,char *output, char *n){
	long pos;  //position in the file
	int i;
	if(!feof(fd)){
		pos = ftell(fd);
		for(i = 0;i < k; i++)
			if( (output[i] = fgetc(fd)) == EOF ) break;
		output[i] = '\0';
		//get the next caracter
		*n = fgetc(fd);

		//We need to rewind the position of the file only to consume one character
		if(!feof(fd)){
			pos += 1;	
			fseek(fd,pos,SEEK_SET);
		}
	}
	else{
		output[0] = '\0';
		*n = EOF;
	}

}


static int CompareLetter(const void *elem1, const void *elem2)
{
    struct frequency *freq1 = (struct frequency *)elem1;
    struct frequency *freq2 = (struct frequency *)elem2;    
    return (freq1->ch - freq2->ch);
}



static int compareHashElement(const void *elem1, const void *elem2)
{
    struct hashelement *he1 = (struct hashelement *)elem1;
    struct hashelement *he2 = (struct hashelement *)elem2;    
    return strcmp(he1->key,he2->key);
}


void generateseeds(char *fname,int k,hashset *seeds){
	FILE *fp;
	char *word;
	char next;
	struct hashelement *found;
	struct  hashelement localHE;
	vector * frecuencies;
	int frecPos;
	struct frequency *newFrec;

	word = (char *)malloc(sizeof(char) * (k + 1));
	fp = fopen(fname,"r");
	slplitkletters(fp,k,word,&next);
	do{
		if(debug) printf("%s > %c|", word,next);
		//intsert the letter on the hashset
		localHE.key = strdup(word);
		found = (struct hashelement *) HashSetLookup(seeds,&localHE);	
		if (found != NULL){ // the elem exist in the hash, add or replace in the vector
			frecuencies = found->value;
			frecPos = VectorSearch(frecuencies, &next, CompareLetter, 0, false);   
			if(frecPos != -1){ //the char is already we need to update the frecuency
				newFrec = (struct frequency *) VectorNth(frecuencies, frecPos);
				newFrec->occurrences += 1;
			}
			else{ //add a new elem with next and occurency equal 1
				newFrec = (struct frequency *)malloc(sizeof(struct frequency));
				newFrec->ch = next;
				newFrec->occurrences = 1;
				VectorAppend(frecuencies, newFrec);
			}
		}
		else{
			//the elem doesn't exist create a new vector and insert it
			localHE.value = (vector *)malloc(sizeof(vector));
			VectorNew(localHE.value,sizeof(struct frequency),NULL,10);
			newFrec = (struct frequency *)malloc(sizeof(struct frequency));
			newFrec->ch = next;
			newFrec->occurrences = 1;
			VectorAppend(localHE.value, newFrec);	
			HashSetEnter(seeds,&localHE);
		}
		slplitkletters(fp,k,word,&next);
	}while(strlen(word) > 0 && next != EOF);
	
	fclose(fp);
	free(word);
}

static const signed long kHashMultiplier = -1664117991L;
static int StringHash(const void *elemAddr, int numBuckets)  
{            
  int i;
  unsigned long hashcode = 0;
  struct hashelement *h = (struct hashelement *)elemAddr;
  char *s = h->key;
  
  for (i = 0; i < strlen(s); i++)  
    hashcode = hashcode * kHashMultiplier + tolower(s[i]);  
  
  return hashcode % numBuckets;                                
}

int main(int argc,char **argv){
	int k,w;
	char *fname;
	hashset seeds;


	//Check the right input
	if(argc != 4){
		usage(argv[0]);
		exit(-1);
	}
	k = atoi(argv[1]);
	fname = strdup(argv[2]);
	w = atoi(argv[3]);
	
	//print the parameters just to confirm
	if(debug) printf("parameters: %d,%s,%d \n", k,fname,w);
	//init the hashset seeds
	HashSetNew(&seeds, sizeof(struct hashelement), 10000,StringHash, compareHashElement, NULL);
	generateseeds(fname,k,&seeds);
	if(debug) HashSetMap(&seeds,printHashElems,stdout);
	HashSetMap(&seeds,genLetters,NULL);
	genrandomWords(&seeds,w);
}