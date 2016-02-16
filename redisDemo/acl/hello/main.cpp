
#include <stdio.h>
#include "acl_cpp/lib_acl.hpp"


/** 
 * @param conn {acl::redis_client&} redis 连接对象 
 * @return {bool} 操作过程是否成功 
 */  
bool test_redis_string(acl::redis_client& conn, const char* key)  
{  
    // 创建 redis string 类型的命令操作类对象，同时将连接类对象与操作类  
    // 对象进行绑定  
    acl::redis_string string_operation(&conn);  
    const char* value = "test_value";  

    // 添加 K-V 值至 redis-server 中  
    if (string_operation.hset(key, value,"nidaye") == false) {  
        const acl::redis_result* res = string_operation.get_result();  
        printf("set key: %s error: %s\r\n", key, res ? res->get_error() : "unknown error");  
        return false;  
    }  
    printf("set key: %s ok!\r\n", key);  
    // 需要重置连接对象的状态，或直接调用 conn.reset() 也可  
    string_operation.clear();  


    /*
    // 添加 K-V 值至 redis-server 中  
    if (string_operation.set(key, value) == false) {  
        const acl::redis_result* res = string_operation.get_result();  
        printf("set key: %s error: %s\r\n", key, res ? res->get_error() : "unknown error");  
        return false;  
    }  
    printf("set key: %s ok!\r\n", key);  
    // 需要重置连接对象的状态，或直接调用 conn.reset() 也可  
    string_operation.clear();  

    // 从 redis-server 中取得对应 key 的值  
    acl::string buf;  
    if (string_operation.get(key, buf) == false) {  
        const acl::redis_result* res = string_operation.get_result();  
        printf("get key: %s error: %s\r\n", key, res ? res->get_error() : "unknown error");  
        return false;  
    }  
    printf("get key: %s ok, value: %s\r\n", key, buf.c_str());  

    // 探测给定 key 是否存在于 redis-server 中，需要创建 redis 的 key  
    // 类对象，同时将 redis 连接对象与之绑定  
    acl::redis_key key_operation;  
    //conn.clear();
    //conn.reset(); // 重置连接状态  
    
    key_operation.set_client(&conn);  // 将连接对象与操作对象进行绑定  
    if (key_operation.exists(key) == false) {  
        if (conn.eof()) { // 检查该连接是否已经结束 
            printf("disconnected from redis-server\r\n");  
            return false;  
        }  

        printf("key: %s not exists\r\n", key);  
    } else  
        printf("key: %s exists\r\n", key);  
    
    // 删除指定 key 的字符串类对象  
    //conn.reset(); // 先重置连接对象状态  
    if (key_operation.del(key, NULL) < 0) {  
        printf("del key: %s error\r\n", key);  
        return false;  
    } else  
        printf("del key: %s ok\r\n", key);  
   */

    conn.close();
    if (conn.eof()) { // 检查该连接是否已经结束 
        printf("disconnected from redis-server\r\n");  
    } else {
        printf("connected from redis-server\r\n");  
    }  
    return true;  
}  

/** 
 * @param redis_addr {const char*} redis-server 服务器地址， 
 *  格式为：ip:port，如：127.0.0.1:6379 
 * @param conn_timeout {int} 连接 redis-server 的超时时间(秒) 
 * @param rw_timeout {int} 与 redis-server 进行通信的 IO 超时时间(秒) 
 */  
bool test_redis(const char* redis_addr, int conn_timeout, int rw_timeout)  
{  
    // 创建 redis 客户端网络连接类对象  
    acl::redis_client conn(redis_addr, conn_timeout, rw_timeout);  
    const char* key = "test_key";  
    return test_redis_string(conn, key);  
} 

int main(){
    //const char* redis_addr = "127.0.0.1:6379";
    const char* redis_addr = "127.0.0.1:6380";
    int conn_timeout = 10, rw_timeout = 10, max_conns = 100;
    
    test_redis(redis_addr,conn_timeout,rw_timeout);
    return 0;
}
