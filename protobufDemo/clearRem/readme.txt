

http://itindex.net/detail/51781-protobuf-%E7%A8%8B%E5%BA%8F-%E5%86%85%E5%AD%98
http://www.smallqiao.com/124833.html



tcmalloc 使用
http://blog.csdn.net/chosen0ne/article/details/9338591


安装步骤
#############################################
http://www.th7.cn/Program/cp/201310/154663.shtml

tar zxvf libunwind-1.1.tar.gz 
cd libunwind-1.1
./configure 
make 
make install 

tar zxvf gperftools-2.1.tar.gz 
cd gperftools-2.1
./configure --enable-frame-pointers
make 
sudo make install 

echo "/usr/local/lib" > /etc/ld.so.conf.d/usr_local_lib.conf
/sbin/ldconfig
