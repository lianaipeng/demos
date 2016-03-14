all:  
	g++ -o client main.cpp -I../lib_acl-3.1/include/ -L../lib_acl-3.1/lib/linux64 -L/usr/lib64 -l_acl_cpp -l_protocol -l_acl -lpthread  -lmysqlcppconn -ldl -lmysqlcppconn-static
clean:
	rm -rf client *.o
