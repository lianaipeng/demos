all: clear sender reciver sender1 reciver1

clear:
	make clean
sender:
	g++ -o sender sender.cpp rabbitmq.cpp  -lrabbitmq
reciver:
	g++ -o reciver reciver.cpp rabbitmq.cpp  -lrabbitmq
sender1:
	g++ -o sender1 sender1.cpp rabbitmq.cpp  -lrabbitmq
reciver1:
	g++ -o reciver1 reciver1.cpp rabbitmq.cpp  -lrabbitmq
	

clean:
	rm -rf  sender reciver sender1 reciver1
