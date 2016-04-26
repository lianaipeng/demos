#include "rabbitmq.h"
//#include <json/json.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

int amqp_error(amqp_rpc_reply_t x, char const *context)
{
    switch (x.reply_type) {
        case AMQP_RESPONSE_NORMAL:
            return 0;
        case AMQP_RESPONSE_NONE:
            //BLT_E << context << " missing RPC reply type!";
            break;

        case AMQP_RESPONSE_LIBRARY_EXCEPTION:
            //BLT_E << context << ":" << amqp_error_string2(x.library_error);
            break;

        case AMQP_RESPONSE_SERVER_EXCEPTION:
            switch (x.reply.id) {
                case AMQP_CONNECTION_CLOSE_METHOD:
                    {
                        amqp_connection_close_t *m = (amqp_connection_close_t *) x.reply.decoded;
                        //BLT_E << context << ":server connection error " << m->reply_code  
                        //    << ", message:" << (int)m->reply_text.len << "." << (char*)m->reply_text.bytes;
                        break;
                    }   
                case AMQP_CHANNEL_CLOSE_METHOD:
                    {
                        amqp_channel_close_t *m = (amqp_channel_close_t *) x.reply.decoded;
                        //BLT_E <<  context << ":server channel error " << m->reply_code 
                        //    << ",message: " << (int)m->reply_text.len << "." << (char *)m->reply_text.bytes;
                        break;
                    }   
                default:
                    //BLT_E << context << ": unknown server error, method id " << x.reply.id;
                    break;
            }   
            break;
    }
    return -1;
}

MessageSender::MessageSender(const std::string& host, const int port, const std::string &username,const std::string &passwd, const std::string &queuename,const std::string &bindingkey) 
    :exchange("nidaye_exchange"), channel_index(5), init_succ(false)
{
    mq_host = host;
    mq_port = port;
    mq_passwd = passwd;
    mq_username = username;
    mq_queuename = queuename; 
    binding_key = bindingkey;

    exchange_type = "direct";
    durable = 1;
}

MessageSender::~MessageSender()
{
    amqp_channel_close(conn,channel_index, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

}

int MessageSender::init()
{
    init_succ = false;

    // 1. 声明一个新的amqp connection
    conn = amqp_new_connection();

    // 2. 获取socket.
    amqp_socket_t *mqsock = NULL;
    mqsock = amqp_tcp_socket_new(conn);
    if(!mqsock)
    {    
        cout << "create rabbit mq error" << endl;
        return -2;
    }    
    int status = amqp_socket_open(mqsock,mq_host.c_str(), mq_port);
    if(status)
    {    
        cout << "opening tcp socket error:" << mq_host << ":" << mq_port <<endl;
        return -3;
    }    

    // 3. 用于登录RabbitMQ server，主要目的为了进行权限管理
    // 第二个参数：虚拟主机，是rabbit-mq进行权限管理的最小单位
    // 第三个参数：最大链接数，此处设成0即可
    // 第四个参数：和客户端通信时所允许的最大的frame size.默认值为131072，增大这个值有助于提高吞吐，降低这个值有利于降低时延
    // 第五个参数：含义未知，默认值填0
    // 第六个参数：用于SSL鉴权
    // 第七八参数：用户名，密码
    amqp_rpc_reply_t mq_reply = amqp_login(conn,"/",0,131072,0, AMQP_SASL_METHOD_PLAIN, mq_username.c_str(), mq_passwd.c_str());
    if(amqp_error(mq_reply, "logging ") != 0)
    {    
        cout << "login rabbitmq error " << "username:" << mq_username << ",passwd:" << mq_passwd << endl;
        return -4;
    }    

    // 4. 用于关联conn和channel
    amqp_channel_open(conn, channel_index);
    if(amqp_error(amqp_get_rpc_reply(conn), "Opening channel") != 0)
    {    
        return -5;
    }    
    
    // 5. exchange声明
    // 第三个参数：exchange
    // 第四个参数：type，"fanout"  "direct" "topic"三选一
    // 第五个参数：passive  
    // 第六个参数：持久化
    amqp_exchange_declare(conn, channel_index, amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes(exchange_type.c_str()), 0, durable, 0, 0, amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn),"Declaring exchange") != 0)
    {
        cout << "exchange" << exchange << " declare error" << endl;
        return -6;
    }
    
    // 6. queue声明
    // 第三个参数：队列名
    // 第四个参数：passive 
    // 第五个参数：队列是否持久化
    // 第六个参数：当前连接不在时，队列是否自动删除
    // 第七个参数：没有consumer时，队列是否自动删除
    // 第八个参数：用于拓展参数，比如x-ha-policy用于mirrored queue。用的amqp_empty_table（不知道含义）
    amqp_queue_declare(conn, channel_index, amqp_cstring_bytes( mq_queuename.c_str()), 0, durable, 0, 0, amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn),"Declaring queue")  != 0)
    {    
        cout << "queue" << mq_queuename << " declare error" << endl;
        return -5;
    }    
    
    // 7. 声明binding
    amqp_queue_bind(conn, channel_index, amqp_cstring_bytes(mq_queuename.c_str()), amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes(binding_key.c_str()), amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn), "bind queue") != 0)
    {    
        return -5;
    }    
    
    amqp_confirm_select(conn,channel_index);  //need confirm
    init_succ = true;
    return 0;
}

int MessageSender::send(const std::string& text)
{
    return sendBody(text);
}
int MessageSender::sendBody(const std::string& body)
{
    if(init_succ == 0)
    {
        return -1;
    }
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    // 发布消息
    // 第四个参数：当exchange为默认“”时，此处填写queue_name，当exchange为direct，此处为binding_key
    // 第五个参数：
    // 第六个参数：更多属性，如何设置消息持久化
    // 第七个参数：怀疑是属性
    // 第八个参数：消息体
    int ret = amqp_basic_publish(conn, channel_index, amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes(binding_key.c_str()), 0, 0, &props, amqp_cstring_bytes(body.c_str()));
    if( ret < 0)
    {    
        cout << "send error" << endl;
        return -1;
    }    


    /* 
     * Publish消息后需要在当前通道上监听返回的信息，来判断消息是否成功投递 
     * 这里要根据投递消息的方式来过滤判断几个方法 
     */  
    {  
        amqp_frame_t frame;  
        amqp_rpc_reply_t ret;  
        struct timeval timeout = { 30, 0};  
        if(AMQP_STATUS_OK != amqp_simple_wait_frame_noblock(conn, &frame,&timeout)) {  
            cout <<"wait time out" << endl;
            return -1;  
        }  

        if (AMQP_FRAME_METHOD == frame.frame_type) {  
            amqp_method_t method = frame.payload.method;  
            switch (method.id) {  
                case AMQP_BASIC_ACK_METHOD: 
                    break;  

                case AMQP_BASIC_NACK_METHOD:  
                    /* if we've turned publisher confirms on, and we've published a message 
                     * here is a message not being confirmed 
                     */  
                    {  
                        amqp_basic_nack_t *s = (amqp_basic_nack_t *) method.decoded;  
                        fprintf(stdout, "NAck.delivery_tag=%d\n", s->delivery_tag);  
                        fprintf(stdout, "NAck.multiple=%d\n", s->multiple);  
                        fprintf(stdout, "NAck.requeue=%d\n", s->requeue);  
                    }  
                    break;  

                case AMQP_BASIC_RETURN_METHOD:  
                    break;  

                case AMQP_CHANNEL_CLOSE_METHOD:  
                    /* a channel.close method happens when a channel exception occurs, this 
                     * can happen by publishing to an exchange that doesn't exist for example 
                     * 
                     * In this case you would need to open another channel redeclare any queues 
                     * that were declared auto-delete, and restart any consumers that were attached 
                     * to the previous channel 
                     */  
                    return -1;  

                case AMQP_CONNECTION_CLOSE_METHOD:  
                    /* a connection.close method happens when a connection exception occurs, 
                     * this can happen by trying to use a channel that isn't open for example. 
                     * 
                     * In this case the whole connection must be restarted. 
                     */  
                    return -1;  

                default:  
                    fprintf(stderr ,"An unexpected method was received %d\n", frame.payload.method.id);  
                    return -1;  
            }  
        }  
    }  
    return 0;
}



MessageReceiver::MessageReceiver(const std::string& host, const int port, const std::string& username, const std::string& passwd, const std::string &queuename,const std::string &bindingkey)
    : mq_host(host), 
    mq_port(port), 
    mq_username(username), 
    mq_passwd(passwd), 
    mq_queuename(queuename), 
    mq_binding_key(bindingkey),
    exchange("nidaye_exchange"), 
    exchange_type("direct"), 
    channel_index(5), 
    init_succ(false), 
    durable(1)
{
}

MessageReceiver::~MessageReceiver()
{
    close();
}

int MessageReceiver::close()
{
    amqp_error(amqp_channel_close(conn, channel_index, AMQP_REPLY_SUCCESS), "amqp_channel_close");
    amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "amqp_connection_close");
    int x = amqp_destroy_connection(conn);
    if(x < 0) {
        const char* errstr = amqp_error_string2(x);
        cout << " amqp_destroy_connection errcode="<< x << " errstr=" << (errstr?errstr:"") << endl;
    }
}

int MessageReceiver::init()
{
    int status = 0;

    conn = amqp_new_connection();

    amqp_socket_t *mqsock = NULL;
    mqsock = amqp_tcp_socket_new(conn);
    if(!mqsock)
    {   
        cout << "create rabbit mq error" << endl;
        return -2; 
    }   
    status = amqp_socket_open(mqsock,mq_host.c_str(), mq_port);
    if(status)
    {   
        const char* errstr = amqp_error_string2(status);
        cout << "opening tcp socket error host="<<mq_host<<" port="<<mq_port<<" errcode="<<status<<" errstr="<< (errstr?errstr:"") << endl;
        return -3; 
    }   
    
    amqp_rpc_reply_t mq_reply = amqp_login(conn,"/",0,131072,0, AMQP_SASL_METHOD_PLAIN, mq_username.c_str(), mq_passwd.c_str());
    if(amqp_error(mq_reply, "logging ") != 0)
    {   
        cout << "login rabbitmq error" <<endl;
        return -4; 
    }   

    amqp_channel_open(conn, channel_index); 
    if(amqp_error(amqp_get_rpc_reply(conn),"Channel open ") != 0)
    {   
        cout << " channel " << channel_index << "open error" << endl;
        return -5; 
    }   

    // qos是 quality of service，我们这里使用主要用于控制预取消息数，避免消息按条数均匀分配，需要和no_ack配合使用
    // 第三个参数：以bytes为单位，0为unlimited
    // 第四个参数：预取的消息条数
    // 第五个参数：global
    int prefetch_count = 1;
    amqp_basic_qos(conn,channel_index,0,prefetch_count,false);
    
    amqp_exchange_declare(conn, channel_index, amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes(exchange_type.c_str()), 0, durable, 0, 0, amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn), "Declare exchange ") != 0)
    {   
        cout << "exchange:" << exchange << " declare error" <<endl;
        return -6; 
    }   
    
    amqp_queue_declare(conn, channel_index, amqp_cstring_bytes(mq_queuename.c_str()), 0, durable, 0, 0, amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn),"Declaring queue")  != 0)
    {    
        cout << "queue " << mq_queuename << "declare error" << endl;
        return -7;
    }
    
    amqp_queue_bind(conn, channel_index, amqp_cstring_bytes(mq_queuename.c_str()), amqp_cstring_bytes(exchange.c_str()), amqp_cstring_bytes(mq_binding_key.c_str()), amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn)," bind queue " ) != 0)
    {
        return -8;
    }
    
    amqp_basic_consume(conn, channel_index, amqp_cstring_bytes(mq_queuename.c_str()),amqp_empty_bytes, 0,0,0,amqp_empty_table);
    if(amqp_error(amqp_get_rpc_reply(conn), "amqp_basic_consume") != 0)
    {
        cout << "amqp_basic_consum error" << endl;
        return -9;
    }
    
    init_succ = true;
    return 0;
}

void* MessageReceiver::run(void* args)
{
    int ret_i = 0;
    amqp_frame_t frame;

    while(true)
    {
        if(!init_succ) {
            ret_i = init();
            if(ret_i != 0) {
                sleep(1);
                cout <<"rabbitmq init failed code="<< ret_i << endl;
                close();
                continue;
            } else {
                init_succ = true;
            }
        }

        amqp_maybe_release_buffers(conn);
        amqp_envelope_t envelope;
        amqp_rpc_reply_t ret = amqp_consume_message(conn,&envelope, NULL,0);
        if(amqp_error(ret, "amqp_consume_message") == 0)
        {
            if(envelope.message.properties._flags & AMQP_BASIC_CONTENT_TYPE_FLAG)
            {
                if(envelope.message.body.len > 0)
                {
                    recv(envelope.message.body.bytes, envelope.message.body.len, args);
                }
            }
            if(amqp_basic_ack(conn,channel_index,envelope.delivery_tag, 0) != 0)
            {
                cout << " faild send ack : " << envelope.delivery_tag << endl;
            }

            amqp_destroy_envelope(&envelope);
        } else {
            init_succ = false;
            close();
        }
    }
    return NULL;
}
