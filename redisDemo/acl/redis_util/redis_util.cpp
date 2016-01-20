
#include "redis_util.h"
#include <stdlib.h>
using namespace RedisUtil;


std::string RedisUtil::toString(double val)
{
    char buf[64];
    gcvt(val,64,buf);   
    return std::string(buf);
}

bool RedisUtil::exists(acl::redis& redis, const std::string& key)
{
    redis.clear();
    if (redis.exists(key.c_str()) == false)
    {
        printf("no exists key: %s\r\n", key.c_str());
        return false;
    }
    else 
        printf("exists key: %s\r\n", key.c_str());
    return true;
}
int RedisUtil::del(acl::redis& redis, const std::string& key)
{
    redis.clear();
    int ret = redis.del_one(key.c_str());
    if (ret < 0)
    {
        printf("del key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else
        printf("del ok, key: %s\r\n", key.c_str());
    return RESULT_OK;
}
int RedisUtil::expire(acl::redis& redis, const std::string& key, int seconds)
{
    redis.clear();
    if (redis.expire(key.c_str(), seconds) < 0)
    {
        printf("expire key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else
        printf("expire ok, key: %s\r\n", key.c_str());
    return RESULT_OK;
}
int RedisUtil::ttl(acl::redis& redis, const std::string& key)
{
    int ttl;
    redis.clear();
    if ((ttl = redis.ttl(key.c_str())) < 0)
    {
        printf("get ttl key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else
        printf("ttl ok, key: %s, ttl: %d\r\n",key.c_str(), ttl);
    return ttl;
}
int RedisUtil::type(acl::redis& redis, const std::string& key)
{
    redis.clear();
    acl::redis_key_t ret = redis.type(key.c_str());
    if (ret == acl::REDIS_KEY_NONE)
    {
        printf("unknown type key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    } 
    else
        printf("type ok, key: %s, ret: %d\r\n", key.c_str(), ret);

    return ret;
}


// ######################### STRING ######################### 
int RedisUtil::set(acl::redis& redis, const std::string& key, const std::string& value)
{
    redis.clear();
    if ( redis.set(key.c_str(), value.c_str()) == false)
    {
        printf("set key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else
        printf("set key: %s ok\r\n", key.c_str());
    return RESULT_OK;
}
int RedisUtil::get(acl::redis& redis, const std::string& key, std::string& value)
{
    acl::string val;
    val.clear();
    redis.clear();
    if (redis.get(key.c_str(), val) == false)
    {
        printf("get key: %s error: %s\r\n", key.c_str(),redis.result_error());
        return REDIS_ERROR;
    } 
    else
        printf("get key: %s ok, val: %s\r\n", key.c_str(),val.c_str());

    value = std::string(val.c_str());
    return true;
}


// ######################### HASH ######################### 
bool RedisUtil::hexists(acl::redis& redis, const std::string& key,const std::string field)
{
    redis.clear();
    if (redis.hexists(key.c_str(), field.c_str()) == false)
    {
        printf("no hexists key: %s\r\n", key.c_str());
        return false;
    }
    else
        printf("hexists key: %s, field: %s\r\n", key.c_str(), field.c_str());

    return true;
}
int RedisUtil::hset(acl::redis& redis, const std::string& key,const std::string field,const std::string value)
{
    redis.clear();
    if (redis.hset(key.c_str(), field.c_str(),value.c_str()) < 0)
    {
        printf("hset key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else 
        printf("hset key: %s ok\r\n", key.c_str());

    return RESULT_OK;
}
int RedisUtil::hget(acl::redis& redis, const std::string& key,const std::string field, std::string& value)
{
    redis.clear();
    acl::string result;
    if (redis.hget(key.c_str(), field.c_str(), result) == false)
    {
        printf("hget key: %s, attr: %s\r\n", key.c_str(), field.c_str());
        return REDIS_ERROR;
    }

    printf("key: %s, field: %s, value: %s, len: %d\r\n", key.c_str(), field.c_str(), value.c_str(), (int) value.length());
    value = result;
    return RESULT_OK;
}

bool RedisUtil::hmset(acl::redis& redis, const std::string& key,const std::map<std::string, std::string>& attrs)
{
    if(key.empty() || attrs.empty())
        return false;

    std::map<acl::string, acl::string> acl_attrs;
    std::map<std::string, std::string>::const_iterator iter = attrs.begin();
    for(; iter != attrs.end(); iter++) {
        acl::string field, value;
        field.format("%s", iter->first.c_str());
        value.format("%s", iter->second.c_str());
        acl_attrs[field] = value;
    } 
    redis.clear();
    if (redis.hmset(key.c_str(), acl_attrs) == false)
    {
        printf("hmset error: %s, key: %s\r\n", redis.result_error(), key.c_str());
        return false;
    }
    else
    {
        printf("hmset ok, key: %s\r\n", key.c_str());
    }
    acl_attrs.clear();
    return true;
}

int RedisUtil::hmget(acl::redis& redis, const std::string& key, std::vector<const char*> fields, std::map<std::string, std::string>& values)
{
    if(!RedisUtil::exists(redis, key.c_str())) {
        return KEY_NO_EXIST; 
    } 
    std::vector<acl::string> result;
    redis.clear();
    if(redis.hmget(key.c_str(), fields, &result) == false) {
        return REDIS_ERROR;
    }
    assert(result.size() == fields.size());
    
    int null_val_count = 0;
    for(int i=0; i<fields.size(); i++) {
        std::string field = fields[i];
        const char* val = result[i].c_str();
        null_val_count += val?0:1;
        std::string value = val?std::string(val):"<null>";
        
        values[field] = value;
    }
    
    if(null_val_count == result.size()) {  // 不存在时，读取到的是null
        return OTHER_ERROR;
    }

    return RESULT_OK;
}
int RedisUtil::hgetall(acl::redis& redis, const std::string& key, std::map<std::string, std::string>& values)
{
    if(!RedisUtil::exists(redis, key.c_str())) {
        return KEY_NO_EXIST;
    }

    redis.clear();
    std::map<acl::string, acl::string> result;
    if(redis.hgetall(key.c_str(), result) == false) {
        return REDIS_ERROR;
    }
    int null_val_count = 0;
    std::map<acl::string, acl::string>::const_iterator iter = result.begin();
    for(; iter != result.end(); iter++) {
        const std::string field = iter->first.c_str();
        const char* val = iter->second.c_str();
        std::string value = val?std::string(val):"<null>";
        null_val_count += val?0:1;

        values[field] = value;
    }

    if(null_val_count == result.size()) {
        return OTHER_ERROR;
    }
    return RESULT_OK;
}
bool RedisUtil::hdel(acl::redis& redis, const std::string& key,const std::string field)
{
    redis.clear();
    int ret = redis.hdel(key.c_str(), field.c_str());
    if (ret < 0)
    {
        printf("hdel key: %s error: %s\r\n", key.c_str(), redis.result_error());
        return REDIS_ERROR;
    }
    else
        printf("hdel ok, key: %s\r\n", key.c_str());
    return RESULT_OK;
}


bool RedisUtil::hsetnx(acl::redis_hash& redis, int n)
{
    acl::string key;
    acl::string attr, value;
    int ret;

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        attr.format("attr4");
        value.format("value_%s", key.c_str());

        redis.clear();
        if ((ret = redis.hsetnx(key.c_str(), attr.c_str(),
                        value.c_str())) <0)
        {
            printf("hsetnx key: %s error: %s\r\n",
                    key.c_str(), redis.result_error());
            return false;
        }
        else if (i < 10)
            printf("hsetnx key: %s ok, ret: %d\r\n",
                    key.c_str(), ret);
    }

    return true;
}




bool RedisUtil::hincrby(acl::redis_hash& redis, int n)
{
    acl::string key, attr;
    long long int result;

    for (int i = 0; i < n; i++)
    {
        key.format("hincr_%s_%d", __keypre.c_str(), i);
        attr.format("attr1");

        redis.clear();
        if (redis.hincrby(key.c_str(), attr.c_str(), 10,
                    &result) == false)
        {
            printf("hincrby error: %s, key: %s, attr: %s\r\n",
                    redis.result_error(), key.c_str(), attr.c_str());
            return false;
        }
        else if (i < 10)
            printf("hincrby, key: %s, attr: %s, result: %lld\r\n",
                    key.c_str(), attr.c_str(), result);
    }

    return true;
}

bool RedisUtil::hincrbyfloat(acl::redis_hash& redis, int n)
{
    acl::string key, attr;
    double result;

    for (int i = 0; i < n; i++)
    {
        key.format("hincrbyfloat_%s_%d", __keypre.c_str(), i);
        attr.format("attr1");

        redis.clear();
        if (redis.hincrbyfloat(key.c_str(), attr.c_str(),
                    8.8, &result) == false)
        {
            printf("hincrbyfloat error: %s, key: %s\r\n",
                    redis.result_error(), key.c_str());
            return false;
        }
        else if (i >= 10)
            continue;

        printf("hincrbyfloat ok, key: %s, attr: %s, result: %.2f\r\n",
                key.c_str(), attr.c_str(), result);
    }

    return true;
}

bool RedisUtil::hkeys(acl::redis_hash& redis, int n)
{
    acl::string key;
    std::vector<acl::string> attrs;

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        attrs.clear();

        redis.clear();
        if (redis.hkeys(key.c_str(), attrs) == false)
        {
            printf("hkeys error: %s, key: %s\r\n",
                    redis.result_error(), key.c_str());
            return false;
        }
        else if (i >= 10)
            continue;

        printf("hkeys ok, key: %s\r\n", key.c_str());
        std::vector<acl::string>::const_iterator cit;
        for (cit = attrs.begin(); cit != attrs.end(); ++cit)
        {
            if (cit != attrs.begin())
                printf(", ");
            printf("%s", (*cit).c_str());
        }
        printf("\r\n");
    }

    return true;
}


bool RedisUtil::hlen(acl::redis_hash& redis, int n)
{
    acl::string key;

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);

        redis.clear();
        int ret = redis.hlen(key.c_str());
        if (ret < 0)
        {
            printf("hlen error: %s, key: %s\r\n",
                    redis.result_error(), key.c_str());
            return false;
        }
        else if (i < 10)
            printf("hlen: %s's value's length: %d\r\n",
                    key.c_str(), ret);
    }

    return true;
}



// ######################### SORTEDSET ######################### 
int RedisUtil::zadd(acl::redis& redis, const std::string& key,const double score,const std::string member)
{
    std::map<acl::string, double> members;
    members[acl::string(member.c_str())] = score;

    redis.clear();
    int ret = redis.zadd(key.c_str(), members);
    if (ret < 0)
    {
        printf("add key: %s error\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else 
        printf("add ok, key: %s, ret: %d\r\n", key.c_str(), ret);

    return RESULT_OK;
}
int RedisUtil::zcard(acl::redis& redis, const std::string& key)
{
    redis.clear();
    int ret = redis.zcard(key.c_str());
    if (ret < 0)
    {
        printf("zcard key: %s error\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zcard ok, key: %s, count: %d\r\n", key.c_str(), ret);

    return ret;
}
int RedisUtil::zcount(acl::redis& redis, const std::string& key,const double min,const double max)
{
    redis.clear();
    int ret = redis.zcount(key.c_str(), min, max);
    if (ret < 0)
    {
        printf("zcount key: %s error\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zcount ok, key: %s, count: %d\r\n", key.c_str(), ret);
    return ret;
}
double RedisUtil::zincrby(acl::redis& redis, const std::string& key,const double increment,const std::string member)
{
    double  result;
    redis.clear();
    if (redis.zincrby(key.c_str(), increment, member.c_str(), &result) == false)
    {
        printf("zincrby error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zincrby ok key: %s, result: %.2f\r\n", key.c_str(), result);
    return result;
}
int RedisUtil::zrange(acl::redis& redis, const std::string& key, std::vector<std::string>& members, int start, int stop, bool isrev)
{
    std::vector<acl::string> result;
    int ret;
    if (!isrev)// 正序
        ret = redis.zrange(key.c_str(), start, stop, &result);
    else 
        ret = redis.zrevrange(key.c_str(), start, stop, &result);

    if (ret < 0)
    {
        printf("zrange error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }

    printf("zrange ok, key: %s, ret: %d\r\n", key.c_str(), ret);
    std::vector<acl::string>::const_iterator cit;
    for (cit = result.begin(); cit != result.end(); ++cit)
    {
        members.push_back(std::string((*cit).c_str())); 
    }
    return ret;
}
int RedisUtil::zrangewithscores(acl::redis& redis, const std::string& key, std::map<std::string,double>& members, int start, int stop, bool isrev)
{
    //std::vector<std::pair<acl::string, double> > result;
    std::vector<std::pair<acl::string, double> > result;

    redis.clear();
    int ret;
    if (!isrev) // 正序
        ret = redis.zrange_with_scores(key.c_str(), start, stop, result);
    else 
        ret = redis.zrevrange_with_scores(key.c_str(), start, stop, result);

    if (ret < 0)
    {
        printf("zrange_with_scores error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    printf("zrange_with_scores ok, key: %s, ret: %d\r\n", key.c_str(), ret);

    std::vector<std::pair<acl::string, double> >::const_iterator cit;
    for (cit = result.begin(); cit != result.end(); ++cit)
    {
        members[std::string(cit->first.c_str())] = cit->second;
    }
    
    return ret;
}
int RedisUtil::zrank(acl::redis& redis, const std::string& key, const std::string member, bool isrev)
{
    redis.clear();
    int ret;
    if(!isrev)
        ret = redis.zrank(key.c_str(), member.c_str());
    else
        ret = redis.zrevrank(key.c_str(), member.c_str());

    if (ret < 0)
    {
        printf("zrank error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zrank ok, key: %s, member:%s, " "rank: %d\r\n", key.c_str(), member.c_str(), ret);
    return ret;
}
int RedisUtil::zrem(acl::redis& redis, const std::string& key, const std::string member)
{
    acl::string tmp_mem;
    tmp_mem.format("%s", member.c_str());
    std::vector<acl::string> members;
    members.push_back(acl::string(member.c_str()));

    redis.clear();
    int ret = redis.zrem(key.c_str(), members);
    if (ret < 0)
    {
        printf("zrem error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zrem ok, key: %s, ret: %d\r\n",key.c_str(), ret);

    return RESULT_OK;
}
int RedisUtil::zremrangebyrank(acl::redis& redis, const std::string& key, int start, int stop)
{
    redis.clear();
    int ret = redis.zremrangebyrank(key.c_str(), start, stop);
    if (ret < 0)
    {
        printf("zremrangebyrank error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zremrangebyrank ok, key: %s, ret: %d\r\n",key.c_str(), ret);

    return RESULT_OK;
}
int RedisUtil::zremrangebyscore(acl::redis& redis, const std::string& key, double min, double max)
{
    redis.clear();
    int ret = redis.zremrangebyscore(key.c_str(), min, max);
    if (ret < 0)
    {
        printf("zremrangebyscore error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else
        printf("zremrangebyscore ok, key: %s, ret: %d\r\n",key.c_str(), ret);

    return RESULT_OK;
}

int RedisUtil::zscore(acl::redis& redis, const std::string& key, const std::string member,double& score)
{
    redis.clear();
    bool ret = redis.zscore(key.c_str(), member.c_str(),score);
    if (ret == false)
    {
        printf("zscore error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }
    else 
        printf("zscore ok, key: %s, member:%s, ""score: %.2f\r\n", key.c_str(), member.c_str(), score);

    return RESULT_OK;
}

int RedisUtil::zrangebyscore(acl::redis& redis, const std::string& key, std::vector<std::string>& members, double min, double max, bool isrev)
{
    std::vector<acl::string> result;

    redis.clear();
    int ret;
    if(!isrev)
        ret = redis.zrangebyscore(key.c_str(), min, max, &result);
//    else
//        ret = redis.zrevrangebyscore(key.c_str(), (const char *)RedisUtil::toString(min).c_str(), (const char *)RedisUtil::toString(max).c_str(), &result);
    if (ret < 0)
    {
        printf("zrangebyscore error, key: %s\r\n", key.c_str());
        return REDIS_ERROR;
    }

    printf("zrangebyscore ok, key: %s, ret: %d\r\n", key.c_str(), ret);
    std::vector<acl::string>::const_iterator cit;
    for (cit = result.begin(); cit != result.end(); ++cit)
    {
        members.push_back(std::string((*cit).c_str())); 
    }

    return ret;
}
int RedisUtil::zrangebyscorewithscores(acl::redis& redis, const std::string& key, std::map<std::string,double>& members, double min, double max, bool isrev)
{
    std::vector<std::pair<acl::string, double> > result;
    redis.clear();

    int ret;
    if(!isrev)
        ret = redis.zrangebyscore_with_scores(key.c_str(),  min, max, result);
    else
        ret = redis.zrevrangebyscore_with_scores(key.c_str(),  min, max, result);

    if (ret < 0)
    {
        printf("zrangebyscore_with_scores error, key: %s\r\n",key.c_str());
        return REDIS_ERROR;
    }

    printf("zrangebyscore_with_scores ok, key: %s, ret: %d\r\n",key.c_str(), ret);
    std::vector<std::pair<acl::string, double> >::const_iterator cit;
    for (cit = result.begin(); cit != result.end(); ++cit)
    {
        members[std::string(cit->first.c_str())] = cit->second;
    }

    return ret;
}











bool RedisUtil::zunionstore(acl::redis_zset& redis, int n)
{
    acl::string dest_key, src1_key, src2_key;
    std::vector<acl::string> src_keys;

    for (int i = 0; i < n; i++)
    {
        dest_key.format("zset_dest_key_%d", i);
        src1_key.format("zset_src1_key_%d", i);
        src_keys.push_back(src1_key);
        src2_key.format("zset_src2_key_%d", i);
        src_keys.push_back(src2_key);

        redis.clear();
        int ret = redis.zunionstore(dest_key.c_str(), src_keys);
        if (ret < 0)
        {
            printf("zunionstore error, dest: %s\r\n",
                    dest_key.c_str());
            return false;
        }
        src_keys.clear();
    }

    return true;
}

bool RedisUtil::zinterstore(acl::redis_zset& redis ,int n)
{
    acl::string dest_key, src1_key, src2_key;
    std::vector<acl::string> src_keys;

    for (int i = 0; i < n; i++)
    {
        dest_key.format("zset_dest_key_%d", i);
        src1_key.format("zset_src1_key_%d", i);
        src_keys.push_back(src1_key);
        src2_key.format("zset_src2_key_%d", i);
        src_keys.push_back(src2_key);

        redis.clear();
        int ret = redis.zinterstore(dest_key.c_str(), src_keys);
        if (ret < 0)
        {
            printf("zinterstore error, dest: %s\r\n",
                    dest_key.c_str());
            return false;
        }
        src_keys.clear();
    }

    return true;
}

bool RedisUtil::zscan(acl::redis_zset& redis, int n)
{
    acl::string key;
    int   ret = 0;
    std::vector<std::pair<acl::string, double> > result;
    std::vector<std::pair<acl::string, double> >::const_iterator cit;

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        while (true)
        {
            redis.clear();
            ret = redis.zscan(key.c_str(), ret, result);
            if (ret < 0)
            {
                printf("zscan failed, key: %s\r\n",
                        key.c_str());
                return false;
            }

            if (i >= 10)
            {
                if (ret == 0)
                    break;
            }

            for (cit = result.begin(); cit != result.end(); ++cit)
            {
                printf("%s: %.2f\r\n", cit->first.c_str(),
                        cit->second);
            }

            if (ret == 0)
            {
                printf("zscan over, key: %s\r\n", key.c_str());
                break;
            }
        }
    }

    return true;
}

bool RedisUtil::zrangebylex(acl::redis_zset& redis, int n)
{
    acl::string key;
    const char* min = "[aaa", *max = "(g";
    std::vector<acl::string> result;

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        redis.clear();
        int ret = redis.zrangebylex(key.c_str(), min, max, &result);
        if (ret < 0)
        {
            printf("zrangebylex error(%s), key: %s\r\n",
                    redis.result_error(), key.c_str());
            return false;
        }
        if (i >= 10)
        {
            result.clear();
            continue;
        }

        std::vector<acl::string>::const_iterator cit;
        for (cit = result.begin(); cit != result.end(); ++cit)
        {
            if (cit != result.begin())
                printf(", ");
            printf("%s", (*cit).c_str());
        }
        printf("\r\n");
    }

    return true;
}

bool RedisUtil::zlexcount(acl::redis_zset& redis, int n)
{
    acl::string key;
    const char* min = "[aaa", *max = "(g";

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        redis.clear();
        int ret = redis.zlexcount(key.c_str(), min, max);
        if (ret < 0)
        {
            printf("zlexcount error, key: %s\r\n", key.c_str());
            return false;
        }
        if (i >= 10)
            continue;
        printf("key: %s, count: %d\r\n", key.c_str(), ret);
    }

    return true;
}

bool RedisUtil::zremrangebylex(acl::redis_zset& redis, int n)
{
    acl::string key;
    const char* min = "[aaa", *max = "(g";

    for (int i = 0; i < n; i++)
    {
        key.format("%s_%d", __keypre.c_str(), i);
        redis.clear();
        int ret = redis.zremrangebylex(key.c_str(), min, max);
        if (ret < 0)
        {
            printf("zremrangebylex error, key: %s\r\n",
                    key.c_str());
            return false;
        }
        if (i >= 10)
            continue;
        printf("key: %s, count: %d\r\n", key.c_str(), ret);
    }

    return true;
}

