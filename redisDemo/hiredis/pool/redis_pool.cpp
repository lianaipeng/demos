
#include "redis_pool.h"

RedisClient::RedisClient(string ip, int port, int timeout)  
{  
    m_timeout = timeout;  
    m_serverPort = port;  
    m_setverIp = ip;  

    //m_beginInvalidTime = 0;  
}
RedisClient::~RedisClient()
{
    while(!m_clients.empty()){  
        redisContext *ctx = m_clients.front();  
        redisFree(ctx);  
        m_clients.pop();  
    } 
}
redisContext* RedisClient::createContext()
{
    if(!m_clients.empty()){
        redisContext *ctx = m_clients.front(); 
        m_clients.pop();
        return ctx;
    }
    time_t now = time(NULL);

    struct timeval tv;  
    tv.tv_sec = m_timeout/1000;  
    tv.tv_usec = (m_timeout%1000)*1000;
    redisContext *ctx = redisConnectWithTimeout(m_setverIp.c_str(), m_serverPort, tv);
    if(ctx == NULL || ctx->err != 0){
        if(ctx != NULL)
            redisFree(ctx);
        return NULL;
    }
    
    return ctx;       
}
void RedisClient::releaseContext(redisContext* ctx,bool active)
{
    if(ctx == NULL)
        return;

    if(!active){
        redisFree(ctx);
        return;
    }
    m_clients.push(ctx);
}
// private 
redisReply* RedisClient::executeCommand(const char* cmd, size_t len)
{
    redisContext *ctx = createContext();  
    if(ctx == NULL) 
        return NULL; 
    
    //redisReply *reply = (redisReply*)redisCommand(ctx, "%b", cmd, len); 
    redisReply *reply = (redisReply*)redisCommand(ctx, "%b", cmd); 
     
    releaseContext(ctx, reply != NULL);
    return reply;

}
// public 
bool RedisClient::executeCommand(const char* cmd, size_t len, string& res)
{
    redisReply *reply = executeCommand(cmd, len);        
    if(reply == NULL)
        return false;

    //boost::shared_ptr<redisReply> autoFree(reply, freeReplyObject); 
    if (reply->type == REDIS_REPLY_INTEGER) {  
        res = IntToString(reply->integer);  
        return true;  
    } else if(reply->type == REDIS_REPLY_STRING) {  
        res.assign(reply->str, reply->len);  
        return true;  
    } else if(reply->type == REDIS_REPLY_STATUS) {  
        res.assign(reply->str, reply->len);  
        return true;  
    } else if(reply->type == REDIS_REPLY_NIL) {  
        res = "";  
        return true;  
    } else if(reply->type == REDIS_REPLY_ERROR) {  
        res.assign(reply->str, reply->len);  
        return false;  
    } else if(reply->type == REDIS_REPLY_ARRAY) {  
        res = "Not Support Array Result!!!";  
        return false;  
    } else {  
        res = "Undefine Reply Type";  
        return false;  
    } 
} 
