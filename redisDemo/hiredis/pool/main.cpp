
#include "redis_pool.h"
#include "redis_util.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

RedisPool *g_pool;

void* monitor_control_fn1(void *){
    std::cout << " in pthread1"  << std::endl;

    sleep(1);
    redisContext* conn = g_pool->getContext();
    std::string tmpKey = "poolset1";
    
    std::cout<< "expire:" << existsRedisKey(conn,"nokey") << std::endl;

    setRedisString(conn,tmpKey,"poolhello1");
    std::cout<< "expire:" << existsRedisKey(conn,tmpKey) << std::endl;

    expireRedisKey(conn,tmpKey,10000); 
    std::cout << "ttl:" << ttlRedisKey(conn,tmpKey) << std::endl;


    std::string  _return="";
    getRedisString(conn,tmpKey,_return);
    std::cout << "######## _return:" << _return << std::endl; 
    g_pool->delContext(conn,true);
    
}
void* monitor_control_fn2(void *){
    std::cout << " in pthread2"  << std::endl;

    sleep(2);
    redisContext* conn = g_pool->getContext();
    std::string tmpKey = "poolset2";
    setRedisString(conn,tmpKey,"poolhello2");

    std::string  _return="";
    getRedisString(conn,tmpKey,_return);
    std::cout << "######## _return:" << _return << std::endl; 
    g_pool->delContext(conn,true);
    
}
int main(){
    g_pool = new RedisPool((char *)"192.168.1.224",6380,5000);

    pthread_t pth;
    //int err = pthread_create(&pth,NULL,(void *)monitor_control_fn(void *),(void *)NULL);
    int err = pthread_create(&pth,NULL,monitor_control_fn1,(void *)NULL);
    if(err != 0){ 
        fprintf(stderr,"%s\n",strerror(err));
    } 
    err = pthread_create(&pth,NULL,monitor_control_fn2,(void *)NULL);
    if(err != 0){ 
        fprintf(stderr,"%s\n",strerror(err));
    } 
    sleep(3);
    
    if(g_pool != NULL)
        delete g_pool;

    /*
	//redisContext* conn = redisConnect((char *)"192.168.1.226",6379); 
	redisContext* conn = redisConnect((char *)"127.0.0.1",6389); 
	if(conn->err)   
		printf("connection error:%s\n",conn->errstr);
	else
		printf("connection succed!\n");
   
    std::string command = "hset testh key1 value2";
    //std::string command = "set test hello";
	redisReply* reply = (redisReply*)redisCommand(conn,command.c_str());
	if(NULL == reply){
		printf("reply error!\n");
	}else{
        printf("reply successed!\n");
        if(reply->type  == REDIS_REPLY_STATUS){
            printf("REDIS_REPLY_STATUS OK\n"); 

            printf("nidayede %s\n",reply->str);
        }else if(reply->type == REDIS_REPLY_ERROR){
            printf("REDIS_REPLY_ERROR OK\n"); 
        }else if(reply->type == REDIS_REPLY_INTEGER){
            printf("REDIS_REPLY_INTEGER OK\n"); 

            printf("nidayede %d\n",reply->integer);
        }else if(reply->type == REDIS_REPLY_NIL){
            printf("REDIS_REPLY_NIL OK\n"); 
        }else if(reply->type == REDIS_REPLY_STRING){
            printf("REDIS_REPLY_STRING OK\n"); 

            printf("nidayede %s\n",reply->len);
        }else if(reply->type == REDIS_REPLY_ARRAY){
            printf("REDIS_REPLY_ARRAY OK\n"); 

            //printf("nidayede %s\n",reply->element[0]);
        } else {
            printf("REDIS_REPLY_STATUS NO\n"); 
        }
    }
    freeReplyObject(reply); 
    
	redisFree(conn); 
    */
	return 0;
}
