all:
	protoc -I=./  --cpp_out=./ addressbook.proto	
	g++ -o test  main.cpp addressbook.pb.cc -lprotobuf -lpthread

clean:
	rm -rf test addressbook.pb.h addressbook.pb.cc
