#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
//#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <pthread.h>

void *cbRead(void *arg){
	printf("###### cbRead ######\n");
	PQueue queue = (PQueue)arg;
	
	int i =4;
	Node node;
	while(1>0){
		popBegin(queue,&node);
		printf("### data %s\n",node.data);
		sleep(2);
		i--;
	}
	
	return (void *)NULL;
}
void *cbWrite(void *arg){
	printf("------ cbWrite ------\n");
	PQueue queue = (PQueue)arg;
	
	int i=10;
	while(i > 0){
		char str[20];
		sprintf(str,"hello%d",i);
		pushEnd(queue,str);
		sleep(1);
		i--;
	}

	return (void *)NULL;
}

int main(){
	Queue Q;
	initQueue(&Q);

	int err;
	/*
	Node node;
	err = popBegin(&Q,&node);
	if(err == 0)
		printf("popBegin :%s\n",node.data);

	pushEnd(&Q,"hello1");
	pushEnd(&Q,"hello2");
	pushEnd(&Q,"hello3");
	pushEnd(&Q,"hello4");
	pushEnd(&Q,"hello5");

	popBegin(&Q,&node);
	printf("popBegin :%s\n",node.data);
	*/
	
	pthread_t write_th,read_th;
	err = pthread_create(&write_th,NULL,cbWrite,(void *)&Q);
	if(err != 0){
		fprintf(stderr,"%s\n",strerror(err));
		exit(1);
	}
	err = pthread_create(&read_th,NULL,cbRead,(void *)&Q);
	if(err != 0){
		fprintf(stderr,"%s\n",strerror(err));
		exit(1);
	}

	sleep(10);
	destroyQueue(&Q);

	return 0;
}
