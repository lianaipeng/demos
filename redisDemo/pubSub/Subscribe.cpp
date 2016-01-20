

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include <unistd.h>

int main(){
	//redisContext* conn = redisConnect((char *)"192.168.1.226",6379); 
	redisContext* conn = redisConnect((char *)"127.0.0.1",6389); 
	if(conn->err)   
		printf("connection error:%s\n",conn->errstr);
	else
		printf("connection succed!\n");
    
    std::string command = "subscribe hello";
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

    sleep(10);

    freeReplyObject(reply); 
    
	redisFree(conn); 
	return 0;
}
