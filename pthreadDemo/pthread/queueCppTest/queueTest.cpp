
#include "MQueue.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <pthread.h>

using namespace std;

void *cbRead(void *){
	printf("###### cbRead ######\n");
	return (void *)NULL;
}
void *cbWrite(void *){
	printf("------ cbWrite ------\n");
	MQueue::pushNode("in cbWrite");

	return (void *)NULL;
}


int main(){
	MQueue queue;
	cout << queue.isEmpty() << endl;

	for(int i=0; i<5 ;i++){
		queue.pushNode("string");
	}
	queue.pushNode("string end");
	MQueue::pushNode("in cbWrite");

	pthread_t write_th;
	int err;
	err = pthread_create(&write_th,NULL,cbWrite,NULL);
	if(err != 0){
		fprintf(stderr,"%s\n",strerror(err));
		exit(1);
	}

	cout << queue.isEmpty() << endl;
	while(!queue.isEmpty()){
		cout << queue.popNode() << endl;
	}

	return 0;
}
