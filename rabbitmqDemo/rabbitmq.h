#ifndef _RABBIT_MQ_H_
#define _RABBIT_MQ_H_

#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <string>
#include <iostream>

using namespace std;

int amqp_error(amqp_rpc_reply_t x, char const *context);

class MessageSender
{
public:
    MessageSender(const std::string& host, const int port, const std::string& username, const std::string& passwd, const std::string &queuename,const std::string &bindingkey);
    ~MessageSender();

    int init();
    int send(const std::string& text);
private:
    std::string mq_host;
    int mq_port;
    std::string mq_passwd;
    std::string mq_username;
    std::string mq_queuename;
    std::string exchange;
    std::string exchange_type;
    std::string binding_key;
    int channel_index;
    int durable;
    int init_succ;

    amqp_connection_state_t conn;

    int sendBody(const std::string& body);
};



class MessageReceiver
{
public:
    MessageReceiver(const std::string& host, const int port, const std::string& username, const std::string& passwd, const std::string &queuename,const std::string &bindingkey);
    virtual ~MessageReceiver();
    
    int init();
    void* run(void* args);
    int close();

protected:
    virtual void recv(void* data, int len, void* args) = 0;

    std::string mq_host;
    int mq_port;
    std::string mq_username;
    std::string mq_passwd;
    std::string mq_queuename;
    std::string mq_binding_key;
    std::string exchange;
    std::string exchange_type;
    int channel_index;
    int durable;

    bool init_succ;

    amqp_connection_state_t conn;
};

#endif
