#include <stdio.h>
#include <stdlib.h>

int main(){
	char *value;
	if((value = getenv("RWDEBUG")) != NULL){
		printf("RWDEBUG = %s\n", value);
	}
	else{
		printf("RWDEBUG doesn't exist\n");
	}
}