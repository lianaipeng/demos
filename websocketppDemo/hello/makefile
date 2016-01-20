
all: clear server client
clear:
	make clean
server:
	g++ -o server Server.cpp -I../ -I/usr/local/include/boost/  -L/usr/local/lib/ -lboost_system -lboost_chrono
client:
	g++ -o client Client.cpp -I../ -I/usr/local/include/boost/  -L/usr/local/lib/ -lboost_system -lboost_chrono -lboost_random -lboost_thread

clean:
	rm -rf server client *.o 
