
# 参考
# https://github.com/zhengshuxin/acl/blob/master/lib_acl_cpp/samples/redis/redis/redis.cpp

# 必须初始化静态变量
# main.cpp
 RedisPool* RedisClient::_pool = NULL;
