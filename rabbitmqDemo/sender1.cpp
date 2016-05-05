
#include <string>
#include <iostream>

#include "rabbitmq.h"

using namespace std;
int main(){
    
    string g_mq_web_host = "192.168.1.222";
    int g_mq_web_port = 5672;
    string g_mq_web_username = "admin";
    string g_mq_web_passwd = "admin";
    string g_mq_web_queuename = "nidaye_queuename1";
    string g_mq_web_routing_key = "nidaye_bindingkey1"; 
    MessageSender* msgSender = NULL;
    msgSender = new MessageSender(g_mq_web_host, g_mq_web_port,g_mq_web_username,g_mq_web_passwd, g_mq_web_queuename,g_mq_web_routing_key);
    if(msgSender->init() != 0){    
        //BLT_E << " error send message source="<<source<<" target="<<target<<" text="<<text;
        delete msgSender;
        return false;
    }

    msgSender->send("1111111111111111");
    cout << "######################" << endl;
    
    delete msgSender;
    
    return 0;
}
