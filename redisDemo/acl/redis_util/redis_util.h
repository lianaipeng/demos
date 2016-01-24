#ifndef __REDIS_UTIL__
#define __REDIS_UTIL__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "acl_cpp/lib_acl.hpp"
#include <vector>
#include <map>

/** 
 * @param redis_addr {const char*} redis-server 服务器地址， 
 *  格式为：ip:port，如：127.0.0.1:6379 
 * @param conn_timeout {int} 连接 redis-server 的超时时间(秒) 
 * @param rw_timeout {int} 与 redis-server 进行通信的 IO 超时时间(秒) 
 */  
class RedisPool
{
//    friend class RedisClient;
public:
    RedisPool():_pool(NULL){
        //printf("#################\n");
    }
    ~RedisPool(){
        if(!_pool){
            delete _pool;
            _pool = NULL;
        }
    }   

    int init(const std::string& redis_addr, int max_conns=100, int conn_timeout=10, int rw_timeout=10)
    {
        //printf("################# init\n");
        _pool = new acl::redis_client_pool(redis_addr.c_str(), max_conns);
        if(!_pool)
        {
            //printf("new redis_client_pool error\n");
            return -1;
        }
        //printf("new redis_client_pool ok\n");
        _pool->set_timeout(conn_timeout,rw_timeout);
        return 0;
    }
    void release()
    {
        if(_pool){
            delete _pool;
            _pool = NULL;
        }
    }
    acl::redis_client * get_client()
    {
        if(!_pool)
            return NULL;
        return (acl::redis_client*)_pool->peek();
    }
    int put_client(acl::redis_client *conn){
        if(!_pool || !conn)
            return -1;
        _pool->put(conn,!conn->eof());
        return 0;
    }
private:
    acl::redis_client_pool* _pool;
};

class RedisClient : public acl::redis
{
public:
    RedisClient(){
        _client = _pool?_pool->get_client():NULL;
        if(_client)
            set_client(_client);
    }
    ~RedisClient(){
        if(_client)
            _pool->put_client(_client);
    }
    static int init_pool(const std::string& redis_addr, int max_conns=100, int conn_timeout=10, int rw_timeout=10)
    {
        if(_pool)
            return 0;
        _pool = new RedisPool();
        //printf("new RedisPool\n");
        if(!_pool)
            return -1;
        return _pool->init(redis_addr, max_conns, conn_timeout, rw_timeout);  
    }
    static void release_pool()
    {
        if(_pool)
            _pool->release();
    }
protected:
    static RedisPool * _pool;
    acl::redis_client * _client;
};


namespace RedisUtil{

#define RESULT_OK       0
#define REDIS_ERROR    -1
#define KEY_NO_EXIST   -2
#define FIELD_NO_EXIST -3
#define OTHER_ERROR    -9

    static acl::string __keypre("zset_key");
std::string toString(double val);
// ######################### KEY ######################### 
bool exists(acl::redis& redis, const std::string& key);
int del(acl::redis& redis, const std::string& key);
int expire(acl::redis& redis, const std::string& key, int seconds);
int ttl(acl::redis& redis, const std::string& key);
/*
 * none (key不存在)
 * string (字符串); list (列表); set (集合); zset (有序集); hash (哈希表)
 */
int type(acl::redis& redis, const std::string& key);


// ######################### STRING ######################### 
int set(acl::redis& redis, const std::string& key, const std::string& value);
int get(acl::redis& redis, const std::string& key, std::string& value);


// ######################### HASH ######################### 
bool hexists(acl::redis& redis, const std::string& key,const std::string field);
int hset(acl::redis& redis, const std::string& key,const std::string field,const std::string value);
int hget(acl::redis& redis, const std::string& key,const std::string field, std::string& value);
bool hmset(acl::redis& redis, const std::string& key,const std::map<std::string, std::string>& attrs);
int hmget(acl::redis& redis, const std::string& key, std::vector<const char*> fields, std::map<std::string, std::string>& values);
int hgetall(acl::redis& redis, const std::string& key, std::map<std::string, std::string>& values);
bool hdel(acl::redis& redis, const std::string& key,const std::string field);


bool hsetnx(acl::redis_hash& redis, int n);
bool hincrby(acl::redis_hash& redis, int n);
bool hincrbyfloat(acl::redis_hash& redis, int n);
bool hkeys(acl::redis_hash& redis, int n);
bool hlen(acl::redis_hash& redis, int n);
// ######################### SORTEDSET ######################### 
int zadd(acl::redis& redis, const std::string& key, double score,const std::string member);
// 返回有序集的总数   不存在的key返回0
int zcard(acl::redis& redis, const std::string& key);
// 返回有序集 key 中， score 值在 min 和 max 之间(默认包括 score 值等于 min 或 max )的成员的数量
int zcount(acl::redis& redis, const std::string& key, double min, double max);
/**
 * 为有序集 key 的成员 member 的 score 值加上增量 increment 
 * 可以通过传递一个负数值 increment ，让 score 减去相应的值，比如 ZINCRBY key -5 member ，就是让 member 的 score 值减去 5 
 * 当 key 不存在，或 member 不是 key 的成员时， ZINCRBY key increment member 等同于 ZADD key increment member 
 * 当 key 不是有序集类型时，返回一个错误
 */
double zincrby(acl::redis& redis, const std::string& key, double increment, std::string member);
/**
 * 返回有序集 key 中，指定区间内的成员,其中成员的位置按 score 值递增(从小到大)来排序
 * 具有相同 score 值的成员按字典序(lexicographical order )来排列
 * 下标参数 start 和 stop 都以 0 为底，也就是说，以 0 表示有序集第一个成员，以 1 表示有序集第二个成员，以此类推。
 * 你也可以使用负数下标，以 -1 表示最后一个成员， -2 表示倒数第二个成员，以此类推
 * 可以通过使用 WITHSCORES 选项，来让成员和它的 score 值一并返回
 */
// 默认返回所有 值递减(从大到小)来排列  isrev = true
int zrange(acl::redis& redis, const std::string& key, std::vector<std::string>& members, int start = 0, int stop = -1, bool isrev = false);  
int zrangewithscores(acl::redis& redis, const std::string& key, std::map<std::string,double>& members, int start = 0, int stop = -1, bool isrev = false);  
// 如果你需要成员按 score 值递减(从大到小)来排列  isrev = true
int zrangebyscore(acl::redis& redis, const std::string& key, std::vector<std::string>& members, double min, double max, bool isrev = false);
int zrangebyscorewithscores(acl::redis& redis, const std::string& key, std::map<std::string,double>& members, double min, double max, bool isrev = false);
// 返回有序集 key 中成员 member 的排名，从0开始。其中有序集成员按 score 值递增(从小到大)顺序排列
int zrank(acl::redis& redis, const std::string& key, const std::string member, bool isrev = false);
int zrem(acl::redis& redis, const std::string& key, const std::string member);
// 移除有序集 key 中，指定排名(rank)区间内的所有成员
int zremrangebyrank(acl::redis& redis, const std::string& key, int start = 0, int stop = -1);
// 移除有序集 key 中，所有 score 值介于 min 和 max 之间(包括等于 min 或 max )的成员
int zremrangebyscore(acl::redis& redis, const std::string& key, double min, double max);
// 返回member的socre
int zscore(acl::redis& redis, const std::string& key, const std::string member,double& score);


bool zunionstore(acl::redis_zset& redis, int n);
bool zinterstore(acl::redis_zset& redis ,int n);
bool zscan(acl::redis_zset& redis, int n);
bool zrangebylex(acl::redis_zset& redis, int n);
bool zlexcount(acl::redis_zset& redis, int n);
bool zremrangebylex(acl::redis_zset& redis, int n);
};
#endif
