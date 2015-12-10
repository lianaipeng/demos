#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *th_fn(void*){
	int i=10;
	while(i>0){
		printf("hello world\n");
		i--;
		sleep(1);
	}
	return (void *)0;
}

void out_state(pthread_attr_t *attr){
	int state;
	if( pthread_attr_getdetachstate(attr,&state) != 0){
		printf("get detachstate failed!\n");
		exit(1);
	}else{
		if(state == PTHREAD_CREATE_JOINABLE){
			printf("joinable thread!\n");
		}else if(state == PTHREAD_CREATE_DETACHED){
			printf("detached thread!\n");
		}else{
			printf("error state!\n");
		}
	}
}

int main(){
	pthread_t pth;
	int err;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	out_state(&attr);

	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	out_state(&attr);

	err = pthread_create(&pth,NULL,th_fn,NULL);
	if(err != 0){
		fprintf(stderr,"%s\n",strerror(err));
	}
	printf("successed\n");

	int i=10;
	while(i>0){
		sleep(1);
		i--;
		printf("###############\n");
	}
	int *ret;
	printf("ret %d\n",ret);

	pthread_attr_destroy(&attr);
	return 0;
}
