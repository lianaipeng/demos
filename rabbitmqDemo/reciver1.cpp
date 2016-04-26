
#include <string>
#include <iostream>

#include "rabbitmq.h"


class MessageReader : public MessageReceiver
{
    public:
        MessageReader(const std::string& host, const int port, const std::string& username, const std::string& passwd, const std::string &queuename,const std::string &bindingkey)
            :MessageReceiver(host, port, username, passwd, queuename, bindingkey)
        {    
        }    
        virtual ~MessageReader() {}
    protected:
        virtual void recv(void* data, int len, void* args)
        {    
            cout << "$$$$$$$$$$$$$$ recv:" << string((char*)data) << endl;
        }
};


using namespace std;
int main(){
    
    string g_mq_web_host = "192.168.1.222";
    int g_mq_web_port = 5672;
    string g_mq_web_username = "admin";
    string g_mq_web_passwd = "admin";
    string g_mq_web_queuename = "nidaye_queuename1";
    string g_mq_web_binding_key = "nidaye_bindingkey";
    MessageReader* msgReader = NULL;
    msgReader = new MessageReader(g_mq_web_host, g_mq_web_port,g_mq_web_username,g_mq_web_passwd, g_mq_web_queuename,g_mq_web_binding_key);
    if(msgReader->init() != 0){    
        //BLT_E << " error send message source="<<source<<" target="<<target<<" text="<<text;
        delete msgReader;
        return false;
    }

    cout << "######################" << endl;
    msgReader->run(NULL);

    delete msgReader;
    return 0;
}
