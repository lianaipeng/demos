
TARGET=server
ROOT_DIR=$(shell pwd)
COMMON_DIR=$(ROOT_DIR)/common
ifeq ($(debug), y)
	CPPFLAGS=-std=c++11 -gdwarf-2 -DDEBUG -DNO_FCGI_DEFINES -DBOOST_LOG_DYN_LINK -Werror -O0 #-pg
else
	CPPFLAGS=-std=c++11 -gdwarf-2 -DNO_FCGI_DEFINES -DBOOST_LOG_DYN_LINK -Werror -O0 #-pg
endif
CXX=/opt/rh/devtoolset-2/root/usr/bin/g++

DEP_LIBS=-ljson-c\
	-ljsoncpp\
	-levent\
	-lcurl\
	-L/usr/lib\
	-L/usr/local/lib\

DEP_INCS=-I.\
	-I/usr/include\
	-I/usr/local/include\
	-I/usr/local/include/boost\
	-I/usr/local/include/hiredis\
	-I/usr/local/include\

DIRS=$(shell find . -maxdepth 3 -type d)
FILES=$(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
OBJS=$(patsubst %.cpp,%.o, $(FILES))

VPATH = .
.cpp.o:
	$(CXX) -c $(CPPFLAGS) $< -o $@ $(DeP_LIBS) $(DEP_INCS)

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX)  -o $@ $(OBJS) $(CPPFLAGS) $(DEP_LIBS) $(DEP_INCS)

clean:
	rm -rf $(TARGET) $(OBJS) $(COMMON_DIR)
