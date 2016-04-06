#include <stdio.h> 
#include <string.h> 

int main () 
{ 
    char str[] ="a,b,c,d*e"; 
    const char * split = ","; 
    char * p; 
    p = strtok(str,split); 
    while(p!=NULL) { 
        printf("%s\n",p); 
        p = strtok(NULL,split); 
    } 

    //getchar(); 
    return 0; 

}
