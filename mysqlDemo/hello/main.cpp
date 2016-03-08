
#include <stdio.h>
#include "acl_cpp/lib_acl.hpp"


////////////////////////////////////////////////////////////////////////////////  
/** 
 * 从数据库中查询表数据 
 * @param db {acl::db_handle&} acl 中的数据库连接句柄引用 
 */  
static void tbl_select(acl::db_handle& db)  
{  
    // 创建 sql 查询语句  
    const char* sql = "select * from group_tbl where"  
        " group_name='test_name' and uvip_tbl='test'";  
    // 查询数据库  
    if (db.sql_select(sql) == false) {  
        printf("select sql: %s error\r\n", sql);  
        return;  
    }  
    printf("\r\n---------------------------------------------------\r\n");  
    // 列出查询结果方法一：从数据库句柄中获得查询结果集合  
    const acl::db_rows* result = db.get_result();  
    if (result)  {  
        // 遍历查询结果集  
        const std::vector<acl::db_row*>& rows = result->get_rows();  
        for (size_t i = 0; i < rows.size(); i++) {  
            const acl::db_row* row = rows[i];  
            // 打印一行结果中的所有结果  
            for (size_t j = 0; j < row->length(); j++)  
                printf("%s, ", (*row)[j]);  
            printf("\r\n");  
        }  
    }  
    // 列出查询结果方法二：根据数组下标遍历数据库句柄的查询结果集  
    for (size_t i = 0; i < db.length(); i++) {  
        const acl::db_row* row = db[i];  

        // 取出该行记录中某个字段的值  
        const char* ptr = (*row)["group_name"];  
        if (ptr == NULL) {  
            printf("error, no group name\r\n");  
            continue;  
        }  
        printf("group_name=%s: ", ptr);  
        for (size_t j = 0; j < row->length(); j++)  
            printf("%s, ", (*row)[j]);  
        printf("\r\n");  
    }  
    // 列出查询结果方法三：直接从数据库句柄中获得结果数组  
    const std::vector<acl::db_row*>* rows = db.get_rows();  
    if (rows) {  
        std::vector<acl::db_row*>::const_iterator cit = rows->begin();  
        for (; cit != rows->end(); cit++) {  
            const acl::db_row* row = *cit;  
            for (size_t j = 0; j < row->length(); j++)  
                printf("%s, ", (*row)[j]);  
            printf("\r\n");  
        }  
    }  
    // 必须释放查询结果  
    db.free_result();  
}

////////////////////////////////////////////////////////////////////////////////  
/** 
 * 打开 mysql 数据库连接句柄 
 * @return {acl::db_handle*} 返回值为 NULL 表示连接数据库失败 
 */  
static acl::db_handle* open_mysql(void)  
{  
    const char* dbaddr = "127.0.0.1:3306";  
    const char* dbname = "acl_test_db";  
    const char* dbuser = "acl_user", *dbpass = "111111";  
    acl::db_handle* db = new acl::db_mysql(dbaddr, dbname, dbuser, dbpass);  

    if (db->open() == false) {  
        printf("open mysql db error\r\n");  
        delete db;  
        return NULL;  
    }  
    return db;  
}


int main(){
    acl::db_handle* db;
    // 操作 mysql 数据库过程  
    db = open_mysql();  
    if (db) {  
        tbl_select(*db);  
        delete db;  
    } 
    /*
    // 操作 sqlite 数据库过程  
    db = open_sqlite();  
    if (db) {  
        tbl_select(*db);  
        delete db;  
    }
    */
    return 0;
}
