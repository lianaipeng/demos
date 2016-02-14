
all: slice client server 
slice:
	slice2cpp --underscore AMIDemo.ice
client:
	c++ -o client Printer.cpp Client.cpp -I. -I/usr/local/Ice-3.6.1/include -L/usr/local/Ice-3.6.1/lib64 -lIce -lIceUtil
server:
	c++ -o server Printer.cpp Server.cpp -I. -I/usr/local/Ice-3.6.1/include -L/usr/local/Ice-3.6.1/lib64 -lIce -lIceUtil

clean:
	rm -rf Printer.h Printer.cpp *.o server client
