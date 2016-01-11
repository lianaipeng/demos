

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <hiredis/hiredis.h>
#include <iostream>
#include <string>

int main(){
	//redisContext* conn = redisConnect((char *)"192.168.1.226",6379); 
	redisContext* conn = redisConnect((char *)"127.0.0.1",6389); 
	if(conn->err)   
		printf("connection error:%s\n",conn->errstr);
	else
		printf("connection succed!\n");
/*
	//redisReply* reply = (redisReply*)redisCommand(conn,"keys *");
    std::string command = "set stest1 value1";
	redisReply* reply = (redisReply*)redisCommand(conn,command.c_str());
	if(NULL == reply){
		printf("reply error!\n");
	}else{
	    printf("reply successed!\n");
        if(reply->type  == REDIS_REPLY_STATUS){
           printf("REDIS_REPLY_STATUS OK\n"); 
            
            printf("nidayede %s\n",reply->str);
        }else{
           printf("REDIS_REPLY_STATUS NO\n"); 
        }
	}
    freeReplyObject(reply); 

    std::string command1 = "get stest1";
	redisReply* reply1 = (redisReply*)redisCommand(conn,command1.c_str());
	if(NULL == reply1){
		printf("reply error!\n");
	}else{
	    printf("reply successed!\n");
        if(reply1->type  == REDIS_REPLY_STRING){
           printf("REDIS_REPLY_STRING OK\n"); 
            
            printf("nidayede %s\n",reply1->str);
        }else{
           printf("REDIS_REPLY_STRING NO\n"); 
        }
	}
    freeReplyObject(reply1); 
*/
   
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
    
     
    printf("######################\n");
    std::string command1 = "rename test testnew";
    //std::string command1 = "zcount p_customerinfo:1158:users_session_list -inf +inf";
    //std::string command1 = "del test";
    //std::string command1 = "type p_customerinfo:1328:users_session_list";
    //std::string command1 = "zrange p_customerinfo:1328:users_session_list 0 -1";
    //std::string command1 = "get test";
    //std::string command1 = "hgetall stesth";
    //std::string command1 = "hgetall p_businessinfo:619";
    //std::string command1 = "keys p_businessinfo:*";
    //std::string command1 = "keys p_businessinfo:-1";
	redisReply* reply1 = (redisReply*)redisCommand(conn,command1.c_str());
	if(NULL == reply1){
		printf("reply1 error!\n");
	}else{
        printf("reply1 successed!\n");
        if(reply1->type  == REDIS_REPLY_STATUS){
            printf("REDIS_REPLY_STATUS OK\n"); 

            printf("nidayede %s\n",reply1->str);
            printf("nidayede %d\n",reply1->integer);
            printf("strcasecmp %d\n",strcasecmp(reply1->str,"zset"));
            printf("strcasecmp %d\n",strcasecmp(reply1->str,"hash"));
        }else if(reply1->type == REDIS_REPLY_ERROR){
            printf("REDIS_REPLY_ERROR OK\n"); 
        }else if(reply1->type == REDIS_REPLY_INTEGER){
            printf("REDIS_REPLY_INTEGER OK\n"); 

            printf("nidayede %d\n",reply1->integer);
        }else if(reply1->type == REDIS_REPLY_NIL){
            printf("REDIS_REPLY_NIL OK\n"); 
        }else if(reply1->type == REDIS_REPLY_STRING){
            printf("REDIS_REPLY_STRING OK\n"); 

            //printf("nidayede %s\n",reply1->len);
            printf("nidayede %s\n",reply1->str);
        }else if(reply1->type == REDIS_REPLY_ARRAY){
            printf("REDIS_REPLY_ARRAY OK\n"); 

            printf("nidayede %d\n",reply1->elements);
            for(int i=0; i<reply1->elements ;i++){
                redisReply* childReply = reply1->element[i];
                if(childReply->type == REDIS_REPLY_STRING){
                    printf("The value is %s\n",childReply->str);
                }
            }
        } else {
            printf("REDIS_REPLY_STATUS NO\n"); 
        }
    }
    freeReplyObject(reply1); 
   

	redisFree(conn); 

	return 0;
}
