#ifndef __COMMON__
#define __COMMON__

#include <map>
#include <iostream>
#include <set>
using namespace std;

#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <boost/assign.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/function.hpp>
#include <boost/regex.hpp>


#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
using namespace boost;

using boost::property_tree::ptree;

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Integer.h>
#include <activemq/util/Config.h>
#include <decaf/util/Date.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h> 
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <thrift/transport/TSocket.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/server/TNonblockingServer.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace ::apache::thrift::concurrency;

#include <redis3m/redis3m.hpp>
#include <redis3m/patterns/patterns.hpp>
using namespace redis3m;

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 
using namespace boost::posix_time;
using namespace boost::gregorian;

#include <json/json.h>

#include "gen-cpp/ChatService.h"
#include "gen-cpp/FrontService.h"
#include "gen-cpp/Throne.h"
#include "gen-cpp/throne_types.h"
#include "gen-cpp/chat_types.h"
#include "gen-cpp/CustomerMessageService.h"

#include <log.h>

extern string g_server_port;

extern string g_customer_service_ip;
extern string g_customer_service_port;

extern string g_hichao_image_service_url;

extern string g_customer_message_service_ip;
extern string g_customer_message_service_port;

extern string g_throne_service_ip;
extern string g_throne_service_port;

extern string g_mysql_connect_str;
extern string g_mysql_user;
extern string g_mysql_passwd;


boost::shared_ptr<throne::ThroneClient> GetThroneServer();
boost::shared_ptr<message::CustomerMessageServiceClient> GetCustomerMessageServer();

#define ROLE_USER                         0
#define ROLE_CUSTOMER                     1

#define MSG_TYPE_TEXT                     0
#define MSG_TYPE_IMAGE                    1
#define MSG_TYPE_GOODS                    2
#define MSG_TYPE_ORDER                    3
#define MSG_TYPE_SYSTEXT                  4
#define MSG_TYPE_LOGIN_NOTIFY             5

#define USER_ONLINE                       0
#define USER_OFFLINE                      1
#define USER_HANG                         2

#define LOGIN_TYPE_SKU                    0
#define LOGIN_TYPE_ORDER                  1

#define HISTORY_TYPE_SELLER_TO_USER_TEXT  0
#define HISTORY_TYPE_USER_TO_SELLER_TEXT  1
#define HISTORY_TYPE_SELLER_TO_USER_IMAGE 2
#define HISTORY_TYPE_USER_TO_SELLER_IMAGE 3
#define HISTORY_TYPE_GOODS                4
#define HISTORY_TYPE_ORDER                5

#define SERVER_MONITOR_TIMEOUT            3600

#define ID_CUSTOMER_TYPE                  1
#define ID_USER_TYPE                      2

#define PLATFORM_CUSTOMER_USERID          1

#define CUSTOMER_SELECT_TIMEOUT           3600
#endif//__COMMON__
