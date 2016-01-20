all:clear server client
clear:
	make clean
server:
	g++ -std=c++11 -o server Server.cpp -I../ -I/usr/local/include/boost/  -L/usr/local/lib/ -lboost_system -lboost_chrono
client:
	#g++ -std=c++11 -o client Client.cpp -I../ -I/usr/local/include/boost/  -L/usr/local/lib/ -lboost_system -lboost_chrono -lboost_random
	g++ -std=c++11 -o client Client.cpp -I../ -I/usr/local/include/boost/  -L/usr/local/lib/ -lboost_system  

clean:
	rm -rf server client *.o 
