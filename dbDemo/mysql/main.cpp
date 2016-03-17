//#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main() {
    sql::Driver *driver;
    sql::Connection *con = NULL;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect("tcp://192.168.1.249:3307", "beta", "f0cf2a92516045024a0c99147b28f05b");
    con->setSchema("customer");
    if(con != NULL)
        cout << "Connected success!" << endl;
    else {
        cout << "Connected failed!" << endl;         
        return -1;     
    }         
    // string sql = "insert into user (id, name, age) values (5, 'duke', 37);";     
    string sql = "SELECT * FROM `message_log` LIMIT 10;";     
    stmt = con->createStatement();
    res = stmt->executeQuery(sql);
    while(res->next()) {
        cout <<  res->getInt("id") << "  " <<  res->getInt("sender") << endl;
    }
    delete res;
    delete stmt;
    delete con;
    return 0;

}
