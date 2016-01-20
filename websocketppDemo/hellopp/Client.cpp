
#include <iostream>
#include <string>
//#include  <boost/thread.hpp> 
#include  <boost/system/error_code.hpp> 

// 是config 的header 档
#include <websocketpp/config/asio_no_tls_client.hpp>
// 是client 的header 档
#include <websocketpp/client.hpp>

// client 端控制的endpoint 型别
typedef websocketpp::client<websocketpp::config::asio_client > client_t;
typedef websocketpp::connection<websocketpp::config::asio_client> handler_t;
//typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// endpoint的在client端基本上是用来控制client的连线用的，一个endpoint是可以建立多个连线的；
// 而每建立一个连线，都会取得一个connection_hdl，这个才是和server沟通时、用来操作的。
// 一般来说流程大致上是：
// 1.建立endpoint
// 2.初始化endpoint
// 3.设置对应的handler
// 4.取得连线、并且建立连线
class WSClient{
    public:
        WSClient(){
            m_endpoint.set_access_channels(websocketpp::log::alevel::all);
            m_endpoint.set_error_channels(websocketpp::log::elevel::all);
            m_endpoint.init_asio();  

            // 可要可不要
            //m_endpoint.set_socket_init_handler(bind(&WSClient::on_socket_init,this,::_1));
            m_endpoint.set_message_handler(bind(&WSClient::on_message,this,::_1,::_2));            
        }
    
        void start(std::string uri) {
            websocketpp::lib::error_code ec;
            client_t::connection_ptr con = m_endpoint.get_connection(uri, ec);

            if(ec) {
                m_endpoint.get_alog().write(websocketpp::log::alevel::app,ec.message());
            }

            //con->set_proxy("http://humupdates.uchicago.edu:8443");
            if(con != NULL)
                m_endpoint.connect(con);
            else{
                return;
            }
            m_endpoint.run();
            std::cout << "endpoint run()" << std::endl;
        }
        void on_socket_init(websocketpp::connection_hdl) {
        }
        void on_message(websocketpp::connection_hdl hdl, client_t::message_ptr) {
            m_endpoint.close(hdl,websocketpp::close::status::going_away,"");
        }
    private:
        client_t m_endpoint;  
};
int main(int argc, char* argv[]) {
    std::string uri = "ws://echo.websocket.org";
    //std::string uri = "192.168.1.224:9002";
        
    /*
    if (argc == 2) {
        uri = argv[1];
    }
    */

    try {
        WSClient endpoint;
        endpoint.start(uri);
        std::cout << "###### end ######" << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
    return 0;
}

