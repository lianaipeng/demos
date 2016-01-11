

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

    std::string command1 = "publish hello nidaye11111111111222333";
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
