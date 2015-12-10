
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <pthread.h>

typedef struct _Node{
	char data[512];
	struct _Node* next;
}Node,*PNode;

typedef struct _Queue{
	PNode front;
	PNode rear;
	pthread_mutex_t mutex;
}Queue,*PQueue;


void initQueue(PQueue Q){
	Q->front = Q->rear = malloc(sizeof(Node));
	Q->front->next = NULL;

	pthread_mutex_init(&Q->mutex,NULL);
}

void destroyQueue(Queue *Q){
	assert(Q != NULL);
	while(Q->front){
		printf("destroy:%s\n",Q->front->data);

		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
	free(Q->rear);
	pthread_mutex_destroy(&Q->mutex);
}

void pushEnd(Queue *Q,const char *str){
	assert(str != NULL && Q != NULL);
	PNode p = malloc(sizeof(Node));
	if(!p){
		exit(1);
	}
	printf("add Node:%s\n",str);
	
	pthread_mutex_lock(&Q->mutex);
	strcpy(p->data,str);
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
	pthread_mutex_unlock(&Q->mutex);
}
int popBegin(PQueue Q,PNode r){
	if(Q->front == Q->rear){
		printf("queue is empty\n");
		return -1;
	}
	
	pthread_mutex_lock(&Q->mutex);
	PNode p;
	p = Q->front->next;
	strcpy(r->data,p->data);

	Q->front->next = p->next;
	if(Q->rear == p)
		Q->rear = Q->front;
	free(p);
	pthread_mutex_unlock(&Q->mutex);

	return 0;
}
#endif
