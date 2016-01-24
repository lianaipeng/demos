all:  
	#g++ -o client main.cpp redis_util.cpp -I../lib_acl/include/ -L../lib_acl/lib/linux64 -l_acl_cpp -l_acl -lpthread
	g++ -o client main.cpp redis_util.cpp -I../lib_acl-3.1/include/ -L../lib_acl-3.1/lib/linux64 -l_acl_cpp -l_acl -lpthread
clean:
	rm -rf client *.o
