CMAKE_MINIMUM_REQUIRED(VERSION 2.6) #cmake最低版本需求，不加入此行会受到警告信息
PROJECT(HELLO) #项目名称　可以随意填写

MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${HELLO_SOURCE_DIR})

#AUX_SOURCE_DIRECTORY(. SRC_LIST) #把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST
#SET(SRC_LIST main.c)
#ADD_EXECUTABLE(hello ${SRC_LIST})  #生成应用程序 hello(在windows下会自动生成hello.exe)
ADD_SUBDIRECTORY(src)
