#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bool.h"

bool debug = true;

void usage(char *s);
void groupWordsBy(FILE *fp, int k, char *word);

int main(int argc, char **argv){
	int k;
	char *name;
	int w;
	FILE *fp;
	char *word;

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
		groupWordsBy(fp, k, word);
		if(debug) printf("<< %s >>\n", word);
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

void groupWordsBy(FILE *fp, int k, char *word){
	int i;
	for(i = 0; i<k; i++){
		word[i] = fgetc(fp);
	}
	word[i] = '\0';
}

// nombre = (char *) malloc(sizeof(char) * (strlen(argv[2]) + 1));
// strcpy(nombre, argv[2]);
// fgets(word, k + 1, fp);
