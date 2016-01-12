
#CXX = g++
#DEP_LIBS=-lthrift\
#		-L/usr/local/lib
#
#
#DEP_INCS=-I/usr/local/include/thrift\
#		./gen-cpp
#
#DIRS=$(shell find . -maxdepth 3 -type d)
#FILES=$(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
#OBJS=$(patsubst %.cpp,%.o, $(FILES))
#
#VPATH = .
#.cpp.o:
#	$(CXX) -c  $< -o $@ $(DEP_LIBS) $(DEP_INCS) 
#
#all: thrift server 
#
#thrift:
#	thrift --gen cpp TTG.thrift
#
#server: $(OBJS)
#	$(CXX)  -o $@ $(OBJS)  $(DEP_LIBS) $(DEP_INCS)
#

all:thrift server client
thrift:
	thrift --gen cpp TTG.thrift
server:
	g++ -o server -I/usr/local/include/thrift -I./gen-cpp -L/usr/local/lib gen-cpp/TTGService.cpp  gen-cpp/TTG_types.cpp gen-cpp/TTG_constants.cpp server.cpp  -lthrift
client:
	g++ -o client -I/usr/local/include/thrift -I./gen-cpp -L/usr/local/lib gen-cpp/TTGService.cpp  gen-cpp/TTG_types.cpp gen-cpp/TTG_constants.cpp client.cpp  -lthrift
clean:
	rm -rf server client *.o gen-cpp/*
