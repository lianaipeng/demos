
#############################################
https://github.com/zhengshuxin/acl/releases
下载源文件

tar -axvf acl-3.1.4.3.tar.gz 
cd acl-3.1.4.3
make all
make install 

cp -r dist ../lib_acl-3.1/

参考 http://zsxxsz.iteye.com/blog/1915779

在include/acl_cpp/db目录下
主要分三个部分：
数据库操作句柄类（db_handle，db_mysql，db_sqlite）
数据库连接池类（db_pool，mysql_pool，sqlite_pool）
数据库服务类（db_service，db_service_mysql，db_service_sqlite，这些类主要用在阻塞非阻塞结合的应用中，如：MFC界面过程与数据库过程的结合，非阻塞IO过程与数据库过程结合）
