
#include <iostream>
#include <string>
#include  <boost/thread.hpp> 
#include  <boost/system/error_code.hpp> 

// 是config 的header 档
#include <websocketpp/config/asio_no_tls_client.hpp>
// 是client 的header 档
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>

// client 端控制的endpoint 型别
typedef websocketpp::client<websocketpp::config::asio_client > client_t;
typedef websocketpp::connection<websocketpp::config::asio_client> handler_t;
//typedef websocketpp::config::asio_tls_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;

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
            m_endpoint.set_socket_init_handler(bind(&WSClient::on_socket_init,this,::_1));
            m_endpoint.set_message_handler(bind(&WSClient::on_message,this,::_1,::_2));            
            m_endpoint.set_open_handler(bind(&WSClient::on_open,this,::_1));
            m_endpoint.set_close_handler(bind(&WSClient::on_close,this,::_1));
            m_endpoint.set_fail_handler(bind(&WSClient::on_fail,this,::_1));
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
            std::cout << "connect " << uri << " is ok!" << std::endl;
            
            m_endpoint.run();
            std::cout << "endpoint run()" << std::endl;
        }
        
        void run_endpoint(){
            m_endpoint.run();
        }
        
        void on_socket_init(websocketpp::connection_hdl) {
            std::cout << "###### on_socket_init()" << std::endl;
        }
        void on_message(websocketpp::connection_hdl hdl, client_t::message_ptr msg) {
            std::cout << "###### on_message()" << std::endl;
            std::cout << msg->get_payload() << std::endl;
            m_endpoint.close(hdl,websocketpp::close::status::going_away,"");
        }
        void on_open(websocketpp::connection_hdl hdl) {
            std::cout << "###### on_open()" << std::endl;
            m_endpoint.send(hdl, "openasdasdfasdf", websocketpp::frame::opcode::text);
        }
        void on_close(websocketpp::connection_hdl) {
            std::cout << "###### on_close()" << std::endl;
        }
        void on_fail(websocketpp::connection_hdl hdl) {
            std::cout << "###### on_fail()" << std::endl;
            client_t::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);

            std::cout << "Fail handler" << std::endl;
            std::cout << con->get_state() << std::endl;
            std::cout << con->get_local_close_code() << std::endl;
            std::cout << con->get_local_close_reason() << std::endl;
            std::cout << con->get_remote_close_code() << std::endl;
            std::cout << con->get_remote_close_reason() << std::endl;
            std::cout << con->get_ec() << " - " << con->get_ec().message() << std::endl;
        }

        
        client_t::connection_ptr get_conmection_hdl(std::string uri){
            websocketpp::lib::error_code ec;
            client_t::connection_ptr con = m_endpoint.get_connection(uri, ec);

            if(ec) {
                m_endpoint.get_alog().write(websocketpp::log::alevel::app,ec.message());
            }

            if(con != NULL)
                m_endpoint.connect(con);
            else{
                return NULL;
            }
            
            return con; 
        }
        //void send_message()
        
    private:
        client_t m_endpoint;
};


int main(int argc, char* argv[]) {
    //std::string uri = "ws://echo.websocket.org";
    std::string uri = "http://192.168.1.224:9002";
        
    /*
    if (argc == 2) {
        uri = argv[1];
    }
    */

    try {
        // 方法一
        /*
        WSClient endpoint;
        endpoint.start(uri);
        */

        // 方法二
        WSClient endpoint;
        client_t::connection_ptr con = endpoint.get_conmection_hdl(uri);
        // Start a thread to run the processing loop
        thread t(bind(&WSClient::run_endpoint,&endpoint));
        std::string sCmd;
        while(1){
            getline(std::cin, sCmd);
            auto ec = con->send( sCmd );  
            std::cout << ec.message() << " sCmd:" << sCmd << std::endl;
            sleep(1);
        }
        t.join();
        
        //boost::thread wsThread(WSClient::run_endpoint);
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

