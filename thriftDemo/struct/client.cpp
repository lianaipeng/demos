// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/TTGService.h"
/*
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
*/
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using boost::shared_ptr;

#include <iostream>

int main(int argc, char **argv) {
    /*
       int port = 9090;
       shared_ptr<TTGServiceHandler> handler(new TTGServiceHandler());
       shared_ptr<TProcessor> processor(new TTGServiceProcessor(handler));
       shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
       shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
       shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

       TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
       server.serve();
       return 0;
       */
    boost::shared_ptr<TSocket> socket(new TSocket("localhost", 19090));  
    boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));  
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));  
    // 只需要改动客户端名字即可
    TTGServiceClient  client(protocol);

    //设置发送、接收、连接超时
    socket->setConnTimeout(2000);
    socket->setRecvTimeout(2000);
    socket->setSendTimeout(2000);
    transport->open();  
    

	// 我们的代码写在这里  
    Request request;
    request.studentId = 10;
    Response _return;
    //void getStudentInfo(Response& _return, const Request& request) 
    client.getStudentInfo(_return,request); 
    //printf("studentId:%d\n",_return.studentId);
	std::cout << "studentId:" << _return.studentId << ",name:" << _return.name << std::endl;

    
	transport->close();  

	return 0;
}

