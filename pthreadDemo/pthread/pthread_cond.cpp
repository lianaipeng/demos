
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void *thread1(void *);
void *thread2(void *);
void *thread3(void *);

int main(void){
	pthread_t t_a,t_b,t_c;
	pthread_create(&t_a,NULL,thread1,(void*)NULL);
	pthread_create(&t_b,NULL,thread2,(void*)NULL);
	pthread_create(&t_c,NULL,thread3,(void*)NULL);
	pthread_join(t_a,NULL);
	pthread_join(t_b,NULL);
	pthread_join(t_c,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	exit(0);
}
void *thread1(void *){
	int i=0;
	for( ;i <3;i++){
		pthread_mutex_lock(&mutex);
		printf("thread1: 1 wait start\n");
		pthread_cond_wait(&cond,&mutex);
		printf("thread1: 1 wait end\n");
		pthread_mutex_unlock(&mutex);

		sleep(1);
	/*	
		pthread_mutex_lock(&mutex);
		printf("thread1: 2 lock\n");
		printf("thread1: 2 broad satrt\n");
		pthread_cond_broadcast(&cond);
		printf("thread1: 2 broad end\n");
		pthread_mutex_unlock(&mutex);
		printf("thread1: 2 unlock\n");
	*/
	}
	/*
	int i=0;
	for( ; i<6 ;i++){
		pthread_mutex_lock(&mutex);
		printf("thread1:lock %d\n",__LINE__);
		if(i%3 == 0){
			printf("thread1:signal 1 %d\n",__LINE__);
			pthread_cond_signal(&cond);
			printf("thread1:signal 2 %d\n",__LINE__);
			sleep(1);
		}
		pthread_mutex_unlock(&mutex);
		printf("thread1: unlock %d\n\n",__LINE__);
		sleep(1);
	}
	*/
}
void *thread2(void *){
	int i=0;
	for( ;i <3;i++){
		pthread_mutex_lock(&mutex);
		printf("thread2: 1 wait start\n");
		pthread_cond_wait(&cond,&mutex);
		printf("thread2: 1 wait end\n");
		pthread_mutex_unlock(&mutex);
		
		sleep(3);
		
		/*
		pthread_mutex_lock(&mutex);
		printf("thread2: 2 lock\n");
		printf("thread2: 2 broad satrt\n");
		pthread_cond_broadcast(&cond);
		printf("thread2: 2 broad end\n");
		pthread_mutex_unlock(&mutex);
		printf("thread2: 2 unlock\n");
		*/
	}
	/*
	while(i<6){
		pthread_mutex_lock(&mutex);
		printf("thread2:lock %d\n",__LINE__);
		if(i%3 != 0){
			printf("thread2:wait 1 %d\n",__LINE__);
			pthread_cond_wait(&cond,&mutex);
			printf("thread2:wait 2 %d\n",__LINE__);
		}
		pthread_mutex_unlock(&mutex);
		printf("thread2:unlock %d\n\n",__LINE__);
		sleep(1);
	}*/
}
void *thread3(void *){
	int i=0;
	for( ;i <3;i++){
		/*
		pthread_mutex_lock(&mutex);
		printf("thread3: 1 lock\n");
		printf("thread3: 1 wait start\n");
		pthread_cond_wait(&cond,&mutex);
		printf("thread3: 1 wait end\n");
		pthread_mutex_unlock(&mutex);
		printf("thread3: 1 unlock\n");
		*/
		sleep(2);
		pthread_mutex_lock(&mutex);
		printf("thread3: 2 broad satrt\n");
		pthread_cond_broadcast(&cond);
		printf("thread3: 2 broad end\n");
		pthread_mutex_unlock(&mutex);
	}
}
