#ifndef __REDIS_UTIL__
#define __REDIS_UTIL__
//#include "common.h"
#include "redis_pool.h"
#include <map>
#include <set>

#define REDIS_NONE 0
#define REDIS_STRING 1
#define REDIS_LIST 2
#define REDIS_SET  3
#define REDIS_ZSET 4
#define REDIS_HASH 5


int getRedisKeys(redisContext* conn,const std::string key,std::set<std::string>& _return);
int getRedisScan(redisContext* conn,const char* match, std::set<std::string>& _return);
int getRedisKeyType(redisContext* conn,const std::string key);
int expireRedisKey(redisContext* conn,const std::string key,const int time);
int delRedisKey(redisContext* conn,const std::string key);
int existsRedisKey(redisContext* conn,const std::string key);

int renameRedisSet(redisContext* conn,const std::string key,const std::string newkey);

int setRedisString(redisContext* conn,const std::string key,const std::string value);
int getRedisString(redisContext* conn,const std::string key,std::string& _return);

int getRedisZset(redisContext* conn,const std::string key,std::set<std::string>& _return);
int remRedisZset(redisContext* conn,const std::string key,std::string value);
int getRedisZsetCount(redisContext* conn,const std::string key);

int delRedisHash(redisContext* conn,const std::string key,const std::string field);
int getRedisHash(redisContext* conn,const std::string key,std::map<std::string,std::string>& _return);
int setRedisHashField(redisContext* conn,const std::string key,const std::string field,const std::string value);
int getRedisHashField(redisContext* conn,const std::string key,const std::string field,std::string& _return);

//redisContext*
#endif 
