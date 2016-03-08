#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <arpa/inet.h> 

#include <iostream>
#include <string>

uint32_t iptoint( const char *ip )
{
    return ntohl( inet_addr(ip) );
}

void inttoip( uint32_t ip_num, char *ip )
{
    std::cout << "inttoip:" << htonl(ip_num) << std::endl;
    
    char* ipaddr=NULL;
    in_addr inaddr;
    inaddr.s_addr= htonl(ip_num);
    ipaddr= inet_ntoa(inaddr);
    
    std::cout << "inttoip:" << std::string(ipaddr) << std::endl;

    strcpy( ip, ipaddr );
}

int main(){
    /*
    float data = 1.236;
    printf("%-10.2f\n",data);
    printf("%+10.2f\n",data);
    char str[32];
    sprintf(str,"%3.2f",data);
    printf("### %s\n",str);
    */

    uint32_t uintip = iptoint("192.168.1.66");
    std::cout << "C++:" << uintip << ":END" << std::endl;
    char ip[16];
    inttoip(uintip,ip);
    std::cout << "C++:" << ip << ":END" << std::endl;
    return 0;
}
