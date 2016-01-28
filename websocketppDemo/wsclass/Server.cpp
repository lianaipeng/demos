#include <iostream>
#include <memory>
#include <map>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server_t;
//typedef websocketpp::server<debug_custom>::connecttion_ptr connection_ptr;
typedef websocketpp::http::parser::request   request_t;
typedef websocketpp::http::parser::response  response_t;


using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

struct connection_data {
    int sessionid;
    std::string name;
};

class WSServer{
  public:
    WSServer():m_next_sessionid(1){
        m_server.init_asio();

        m_server.set_open_handler(bind(&WSServer::on_open,this,::_1));
        m_server.set_close_handler(bind(&WSServer::on_close,this,::_1));
        m_server.set_message_handler(bind(&WSServer::on_message,this,::_1,::_2));
        m_server.set_http_handler(bind(&WSServer::on_http,this,::_1));
    }
    void on_open(connection_hdl hdl) {
        std::cout << "###### on_open()" << std::endl;
        connection_data data;

        data.sessionid = m_next_sessionid++;
        data.name = "";

        m_connections[hdl] = data;
    }
    void on_close(connection_hdl hdl) {
        std::cout << "###### on_close()" << std::endl;
        connection_data& data = get_data_from_hdl(hdl);

        std::cout << "Closing connection " << data.name << " with sessionid " << data.sessionid << std::endl;

        m_connections.erase(hdl);
    }
    void on_message(connection_hdl hdl, server_t::message_ptr msg) {
        std::cout << "###### on_message()" << std::endl;
        connection_data& data = get_data_from_hdl(hdl);
        server_t::connection_ptr con = m_server.get_con_from_hdl(hdl);
        std::cout << "msg->get_payload():" << msg->get_payload() << std::endl;
        
        //con->send("nidaye",msg->get_opcode());
        m_server.send(hdl,msg->get_payload(),msg->get_opcode());
        /*
        server_t::connection_ptr con = m_server.get_con_from_hdl(hdl);
        request_t req = con->get_request(); 
        std::cout << "nidaye##### uri:" << req.get_uri()<< ",method:" << req.get_method() << ",body:" << req.get_body() << std::endl; 
        */
        if (data.name == "") {
            data.name = msg->get_payload();
            std::cout << "Setting name of connection with sessionid " << data.sessionid << " to " << data.name << std::endl;
        } else {
            std::cout << "Got a message from connection " << data.name << " with sessionid " << data.sessionid << std::endl;
        }
    }
    void on_http(connection_hdl hdl) {
        std::cout << "###### on_http()" << std::endl;
        server_t::connection_ptr con = m_server.get_con_from_hdl(hdl);
        request_t req = con->get_request(); 
        const std::string& strUri     = req.get_uri();
        const std::string& strMethod  = req.get_method();
        const std::string& strBody    = req.get_body();
        const std::string& strVersion = req.get_version();
        std::cout << " uri:" << strUri<< ",method:" << strMethod << ",body:" << strBody << ",version:" << strVersion << std::endl; 

        // 返回curl请求
        con->set_body("everything is ok now!");
        con->set_status(websocketpp::http::status_code::value(600));
        /*
        response_t rsp;
        std::string strContent = req.raw();
        rsp.consume(strContent.c_str(),strContent.size());
        */
    }
    connection_data& get_data_from_hdl(connection_hdl hdl) {
        con_list::iterator it = m_connections.find(hdl);

        if (it == m_connections.end()) {
            // this connection is not in the list. This really shouldn't happen
            // and probably means something else is wrong.
            throw std::invalid_argument("No data avaliable for session");
        }

        return it->second;
    }
    void start(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }
  private:
    typedef std::map<connection_hdl,connection_data,std::owner_less<connection_hdl> > con_list;

    int m_next_sessionid;
    server_t m_server;
    con_list m_connections;
};


int main() {
    WSServer print_server;
    //server print_server;

    print_server.start(9002);
}
