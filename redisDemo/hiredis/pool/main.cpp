
#include "redis_pool.h"
#include <iostream>


int main(){
    RedisClient client((char *)"127.0.0.1",6389,10000);
    std::string cmd = "set test hello";
    std::string res;
    bool ret = client.executeCommand(cmd.c_str(),cmd.size(),res);
    std::cout << "cmd:" << cmd << " cmd.size():" << cmd.size()<< " ret:" << ret << " res:" <<  res << std::endl;
    
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
