#include "common.h"



boost::shared_ptr<throne::ThroneClient> GetThroneServer(){
  boost::shared_ptr<TSocket> socket(new TSocket(g_throne_service_ip, lexical_cast<int>(g_throne_service_port)));
  socket->setConnTimeout(1000);
  socket->setRecvTimeout(1000);
  socket->setSendTimeout(1000);
  
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  boost::shared_ptr<throne::ThroneClient> pclient(new throne::ThroneClient(protocol));
  
  transport->open();
  return pclient;    
}

boost::shared_ptr<message::CustomerMessageServiceClient> GetCustomerMessageServer() {
  boost::shared_ptr<TSocket> socket(new TSocket(g_customer_message_service_ip, lexical_cast<int>(g_customer_message_service_port)));
  socket->setConnTimeout(1000);
  socket->setRecvTimeout(1000);
  socket->setSendTimeout(1000);
  
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  boost::shared_ptr<message::CustomerMessageServiceClient> pclient(new message::CustomerMessageServiceClient(protocol));
  
  transport->open();
  return pclient;
}
