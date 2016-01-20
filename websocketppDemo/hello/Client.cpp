
#include <iostream>
#include <string>
#include  <boost/thread.hpp> 
#include  <boost/system/error_code.hpp> 

// config 的hesrder 档
#include <websocketpp/config/asio_no_tls_client.hpp>
// 是client 的header 档
#include <websocketpp/client.hpp>

// client 端控制的endpoint 型别
typedef websocketpp::client<websocketpp::config::asio_client > WSClient;

/*
void on_message(websocketpp::connection_hdl hdl, WebServer::message_ptr msg) {
    std::cout << msg->get_payload() << std::endl;
}
*/
void on_message(websocketpp::connection_hdl hdl , WSClient::message_ptr msg) {
    std::cout << msg->get_payload() << std::endl;
}
// endpoint的在client端基本上是用来控制client的连线用的，一个endpoint是可以建立多个连线的；
// 而每建立一个连线，都会取得一个connection_hdl，这个才是和server沟通时、用来操作的。
// 一般来说流程大致上是：
// 1.建立endpoint
// 2.初始化endpoint
// 3.设置对应的handler
// 4.取得连线、并且建立连线

// 1. cretae endpoint   
WSClient   mEndPoint;
void testrun(){
    mEndPoint.run();
    return ;
}
int main() {
    //std::string sUrl = "ws://echo.websocket.org";
    std::string sUrl = "http://192.168.1.224:9002";
    // 1. cretae endpoint   
    // WSClient   mEndPoint;

    // 2. initial endpoind  
    mEndPoint.init_asio();  
    mEndPoint.start_perpetual();  
    //boost::thread wsThread( [&mEndPoint](){ mEndPoint.run (); } ); 
    boost::thread wsThread(testrun); 
    //mEndPoint.run();  

    // 3. set handler   
    mEndPoint.set_message_handler(&on_message);
    
    // 4. get connection and connect   
    websocketpp::lib::error_code ec;  
    WSClient::connection_ptr wsCon = mEndPoint.get_connection( sUrl, ec);  
    mEndPoint.connect( wsCon ); 

    std::string sCmd;  
    while ( true )  
    {  
        std::cout << "Enter Command: " << std::flush;  
        getline(std::cin, sCmd);  

        if ( sCmd == "quit" )  
        {  
            wsCon->close( 0, "close" );  
            break ;  
        }  

        //auto ec = wsCon->send( sCmd );  
        ec = wsCon->send( sCmd );  
        std::cout << ec.message() << std::endl;  
    }  
    
    mEndPoint.stop_perpetual();  
    wsThread.join(); 
    /*
    WebServer print_server;
    //server print_server;

    print_server.set_message_handler(&on_message);

    print_server.init_asio();
    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
    */
    return 0;
}
