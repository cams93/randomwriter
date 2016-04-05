#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bool.h"

bool debug = true;
typedef struct hE{
	char *key;
	vector *value;
} hashelem_t;

typedef struct vE{
	char c;
	int frecuency;
} vecelem_t

void usage(char *s);
void groupWordsBy(FILE *fp, int k, char *word, char *next);
void getSeeds (char *fname, int k, hashset *seeds);

int main(int argc, char **argv){
	int k;
	char *name;
	int w;
	FILE *fp;
	char *word;
	char next;

	if(argc != 4){
		usage(argv[0]);
		exit(-1);
	}
	//Read argumnets
	k = atoi(argv[1]);
	name = strdup(argv[2]);
	w = atoi(argv[3]);
	if(debug) printf("<< parameters %d %s %d >>\n", k, name, w);
	fp = fopen(name, "r");
	assert(fp != NULL && "file doesn't exist\n");
	word = (char *) malloc(sizeof(char) * (k + 1));
	while(!feof(fp)){
		groupWordsBy(fp, k, word , &next);
		if(debug) printf("<< %s : %c >>\n", word, next);
	}

	fclose(fp);
	free(name);
	free(word);
	return 0;
}

void usage(char *s){
	printf("Error: wrong number of arguments\n"
		"Use: %s k name w:\n"
		"Where: \n"
			"\t k: number of letters\n"
			"\t name: name of the file\n"
			"\t w: number of words\n", s
		);
}

void groupWordsBy(FILE *fp, int k, char *word, char *next){
	int i;
	long file_pos;
	file_pos = ftell(fp);
	for(i = 0; i<k; i++){
		if ((word[i] = fgetc(fp)) == EOF) break;
	}
	word[i] = '\0';
	if(!feof(fp)) {
		*next = fgetc(fp);
		fseek(fp, file_pos + 1, SEEK_SET);
	} 
}

getSeeds (char *fname, int k, hashset *seeds){

}

// nombre = (char *) malloc(sizeof(char) * (strlen(argv[2]) + 1));
// strcpy(nombre, argv[2]);
// fgets(word, k + 1, fp);
