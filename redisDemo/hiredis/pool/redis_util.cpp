#include "redis_util.h"
#include <string.h>

int getRedisKeys(redisContext* conn,const std::string key,std::set<std::string>& _return){
    redisReply* reply = (redisReply*)redisCommand(conn,"keys %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_ARRAY){
        for(int i=0; i<reply->elements ;i++){
            redisReply* childReply = reply->element[i];
            if(childReply->type == REDIS_REPLY_STRING){
                //printf("The value is %s\n",childReply->str);
                _return.insert(childReply->str);
            }   
            //freeReplyObject(childReply); 
        }
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}

int getRedisScan(redisContext* conn,const char* match, std::set<std::string>& _return){
    std::string iter;
    do {
        redisReply* reply = (redisReply*)redisCommand(conn, "scan %s match %s count 100", iter.c_str(), match);
        if(!reply) {
            //BLT_D << "reply error";
            return -1;
        }
        iter = reply->element[0]->str;
        redisReply* member = reply->element[1];
        for(int i = 0; i < member->elements; ++i){
            _return.insert(member->element[i]->str);
        }
        freeReplyObject(reply);
    }while(iter[0] != '0');
    return 0;
}

int getRedisKeyType(redisContext* conn,const std::string key)
{
    redisReply* reply = (redisReply*)redisCommand(conn,"type %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    int type = -1;
    if(reply->type == REDIS_REPLY_STATUS){
        if( strcasecmp(reply->str,"none") == 0){
            type = REDIS_NONE;
        }else if(strcasecmp(reply->str,"string") == 0){
            type = REDIS_STRING;
        }else if(strcasecmp(reply->str,"list") == 0){
            type = REDIS_LIST;
        }else if(strcasecmp(reply->str,"set") == 0){
            type = REDIS_SET;
        }else if(strcasecmp(reply->str,"zset") == 0){
            type = REDIS_ZSET;
        }else if(strcasecmp(reply->str,"hash") == 0){
            type = REDIS_HASH;
        }
    }
    
    freeReplyObject(reply); 
    return type;
}
int expireRedisKey(redisContext* conn,const std::string key,const int time)
{
    redisReply* reply = (redisReply*)redisCommand(conn,"expire %s %d",key.c_str(),time);
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_INTEGER){
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}

int delRedisKey(redisContext* conn,const std::string key)
{
    //BLT_D << "del " << key;
    redisReply* reply = (redisReply*)redisCommand(conn,"del %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_INTEGER){
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}

int existsRedisKey(redisContext* conn,const std::string key)
{
    redisReply* reply = (redisReply*)redisCommand(conn,"exists %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_INTEGER){
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
    
}

int renameRedisSet(redisContext* conn,const std::string key,const std::string newkey)
{
    //BLT_D << "rename " << key << " " << newkey;
    redisReply* reply = (redisReply*)redisCommand(conn,"rename %s %s",key.c_str(),newkey.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_STATUS){
        //std::cout << " rename ok" << std::endl;
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}

int setRedisString(redisContext* conn,const std::string key,const std::string value){
    redisReply* reply = (redisReply*)redisCommand(conn,"set %s %s",key.c_str(),value.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }
    
    if(reply->type == REDIS_REPLY_STATUS){
        freeReplyObject(reply); 
        return 0;
    }

    freeReplyObject(reply); 
    return 1;
}
int getRedisString(redisContext* conn,const std::string key,std::string& _return){
    redisReply* reply = (redisReply*)redisCommand(conn,"get %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_STRING){
        _return = std::string(reply->str);
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}

int getRedisZset(redisContext* conn,const std::string key,std::set<std::string>& _return){
    redisReply* reply = (redisReply*)redisCommand(conn,"zrange %s 0 -1",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_ARRAY){
        for(int i=0; i<reply->elements ;i++){
            redisReply* childReply = reply->element[i];
            if(childReply->type == REDIS_REPLY_STRING){
                //printf("The value is %s\n",childReply->str);
                _return.insert(childReply->str);
            }   
            //freeReplyObject(childReply); 
        }
        freeReplyObject(reply); 
        return 0;
    }
    
    freeReplyObject(reply); 
    return 1;
}
int remRedisZset(redisContext* conn,const std::string key,std::string value){
    //BLT_D << "zrem " << key << " " << value;
    //std::cout << "zrem " << key << "   " << value << std::endl;;
    redisReply* reply = (redisReply*)redisCommand(conn,"zrem %s %s",key.c_str(),value.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    int count = 0;
    if(reply->type == REDIS_REPLY_INTEGER){
        count = reply->integer;
        freeReplyObject(reply); 
    }
    
    return count;
}
int getRedisZsetCount(redisContext* conn,const std::string key){
    //redisReply* reply = (redisReply*)redisCommand(conn,"zcount %s:users_session_list -inf +inf",key.c_str());

    printf("############### %s\n",key.c_str());
    redisReply* reply = (redisReply*)redisCommand(conn,"zcard %s:users_session_list",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return 0;
    }

    int count = reply->type == REDIS_REPLY_INTEGER ? reply->integer : 0;

    freeReplyObject(reply); 
    return count;
}




int delRedisHash(redisContext* conn,const std::string key,const std::string field){
    return 0;
}
int getRedisHash(redisContext* conn,const std::string key,std::map<std::string,std::string>& _return){
    redisReply* reply = (redisReply*)redisCommand(conn,"hgetall %s",key.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_ARRAY && reply->elements >0 && reply->elements%2 == 0){
        for(int i=0; i<reply->elements ;i=i+2){
            redisReply* keyReply = reply->element[i];
            redisReply* valueReply = reply->element[i+1];
            if(keyReply->type == REDIS_REPLY_STRING && valueReply->type == REDIS_REPLY_STRING){
                //printf("The key is %s,%s\n",keyReply->str,valueReply->str);
                _return[keyReply->str] = valueReply->str;
            }   
        }
    }
    
    freeReplyObject(reply); 
    return _return.size();
}
int setRedisHashField(redisContext* conn,const std::string key,const std::string field,const std::string value)
{
    //BLT_D << "hset " << key << " " << field << " " << value ; 
    redisReply* reply = (redisReply*)redisCommand(conn,"hset %s %s %s",key.c_str(),field.c_str(),value.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }
    
    if(reply->type == REDIS_REPLY_INTEGER){
        //std::cout << "modify ok" << std::endl;
        freeReplyObject(reply); 
        return 0;
    }

    freeReplyObject(reply); 
    return -1;
}
int getRedisHashField(redisContext* conn,const std::string key,const std::string field,std::string& _return)
{
    redisReply* reply = (redisReply*)redisCommand(conn,"hget %s %s",key.c_str(),field.c_str());
    if(NULL == reply){
        //BLT_D << "reply error";
        return -1;
    }

    if(reply->type == REDIS_REPLY_NIL){
        ////BLT_D << "REDIS_REPLY_NIL";
        freeReplyObject(reply); 
        return 1; // 操作redis成功，但是该字段为空
    }

    if(reply->type == REDIS_REPLY_STRING){
        ////BLT_D << "REDIS_REPLY_STRING " << reply->str ;
        _return = reply->str;
    }
    
    freeReplyObject(reply); 
    return 0;
}
