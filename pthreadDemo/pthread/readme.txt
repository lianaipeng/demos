
void *th_fn(void*){
	int i=10;
	while(i>0){
		printf("hello world\n");
		i--;
		sleep(1);
	}
	return (void *)0;
}

int main(){
	err = pthread_create(&pth,NULL,th_fn,NULL);   // C 
	err = pthread_create(&pth,NULL,th_fn,(void *)NULL);   // C 
	if(err != 0){
		fprintf(stderr,"%s\n",strerror(err));
	}
}
