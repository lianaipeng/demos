all: clear test
clear:
	make clean
test:
	g++ -std=c++11 -DBOOST_LOG_DYN_LINK -o test main.cpp -I/usr/local/include/boost -L/usr/local/lib -lboost_log_setup -lboost_log -lboost_thread -lboost_filesystem -lboost_date_time -lboost_iostreams -lboost_system -lboost_regex -lpthread

clean:
	rm -rf *.o test
