all:  
	/opt/rh/devtoolset-2/root/usr/bin/g++ -o client main.cpp -I../lib_acl-3.1/include/ -L/usr/lib64/ -L../lib_acl-3.1/lib/linux64 -l_acl_cpp -l_protocol -l_acl -lpthread  -lmysqlcppconn -ldl -lmysqlclient_r
clean:
	rm -rf client *.o
