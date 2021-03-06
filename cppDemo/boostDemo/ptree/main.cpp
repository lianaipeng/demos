#include <string>  
#include <iostream>  
#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
using namespace boost::property_tree;

int main(){  
    boost::property_tree::ptree pt;
    //boost::property_tree::read_xml("del.conf", pt); 
    boost::property_tree::read_ini("config.ini", pt);

    cout << pt.get<std::string>("mysql.ip","192.168.1.224") << endl;
    cout << pt.get<int>("mysql.port",6666) << endl;

    return 0;  
} 
