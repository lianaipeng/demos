

#include <stdio.h>
#include <stdlib.h>

int main(){
	int a =100;
	char string[25];
	sprintf(string,"hello%d",a);

	printf(" %s \n",string);
	
	return 0;
}
