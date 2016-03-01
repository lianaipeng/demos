#include <stdio.h>

#include <iostream>
#include <string>

int main(){
    float data = 1.236;
    printf("%-10.2f\n",data);
    printf("%+10.2f\n",data);
    char str[32];
    sprintf(str,"%3.2f",data);
    printf("### %s\n",str);

    std::cout << "C++ " << std::string(str) << "END" << std::endl;
    return 0;
}
