#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__

#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
//#include <stddef.h>
//#include <stdarg.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;
static string IntToString(long n){
    // int 转 string
    stringstream ss;
    string str;
    ss<<n;
    ss>>str;
    return str;
}

class RedisClient{
    public:
        RedisClient(std::string ip, int port,int timeout=2000);
        virtual ~RedisClient();

        bool executeCommand(const char* cmd, size_t len, string& res);        
    private:  
        int m_timeout;  
        int m_serverPort;  
        string m_setverIp; 
        queue<redisContext *> m_clients; 

        redisContext* createContext();
        void releaseContext(redisContext* ctx,bool active);
        redisReply* executeCommand(const char* cmd, size_t len);        
};

#endif
