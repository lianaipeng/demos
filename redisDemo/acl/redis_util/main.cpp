
#include <stdio.h>
#include "redis_util.h"
#include <time.h> 
#include <iostream>

RedisPool* RedisClient::_pool = NULL;

int main(){
    int ret;
    const std::string tmp_key = "test_key";
    const std::string tmp_zkey = "test_zkey";
    const std::string tmp_hkey = "test_hkey";
    
    std::string redis_addr = "127.0.0.1:6380";
    ret = RedisClient::init_pool(redis_addr);
    std::cout << "RedisClient::init_pool ret:" << ret << std::endl;

    RedisClient redis;
    RedisUtil::set(redis,tmp_key,"isok?");
    ret = RedisUtil::type(redis,tmp_key);
    std::cout << "############## set ret:" << ret << std::endl;
    RedisUtil::hset(redis,tmp_hkey,"hfield1","isok?");
    RedisUtil::hset(redis,tmp_hkey,"hfield2","isok?");
    ret = RedisUtil::type(redis,tmp_hkey);
    std::cout << "############## hset ret:" << ret << std::endl;
    RedisUtil::zadd(redis,tmp_zkey,1,"zmemeber");
    ret = RedisUtil::type(redis,tmp_zkey);
    std::cout << "############## zset ret:" << ret << std::endl;
    
    RedisClient::release_pool();
    return 0;
}
