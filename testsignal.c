#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void signalhandled(int v){
	printf("signal %d \n", v);
}

void signalcontrolc(int v){
	printf("jajaja %d \n", v);
}

int main(){
	signal(10, signalhandled);
	signal(9, signalcontrolc);
	while(1){
		printf("Waiting for a signal ...\n");
		sleep(1);
	}
	return 0;
}