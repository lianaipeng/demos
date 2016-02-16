
#include "redis_pool.h"

RedisPool::RedisPool(string ip, int port, int timeout)  
{  
    m_timeout = timeout;  
    m_serverPort = port;  
    m_setverIp = ip;  

    //m_beginInvalidTime = 0;  
}
RedisPool::~RedisPool()
{
    while(!m_clients.empty()){  
        redisContext *ctx = m_clients.front();  
        redisFree(ctx);  
        m_clients.pop();  
    } 
}
redisContext* RedisPool::getContext()
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
    std::cout << "m_clients.size():" << m_clients.size() << std::endl;
    
    return ctx;       
}
void RedisPool::delContext(redisContext* ctx,bool active)
{
    if(ctx == NULL)
        return;

    if(!active){
        redisFree(ctx);
        return;
    }
    m_clients.push(ctx);
    std::cout << "m_clients.size():" << m_clients.size() << std::endl;
}


