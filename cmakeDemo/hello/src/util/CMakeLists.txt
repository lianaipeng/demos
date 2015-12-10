SET(LIBRARY_OUTPUT_PATH ${HELLO_SOURCE_DIR}/lib)  # 定义了库生成的路径，LIBRARY_OUTPUT_PATH是一个内部变量，存放库生成路径
#SET(CMAKE_C_COMPILER g++)
SET(SRC_LIST hello.c)

INCLUDE_DIRECTORIES(${HELLO_SOURCE_DIR}/include)  # 是用来定义非标准库头文件要搜索的路径
ADD_LIBRARY(util STATIC ${SRC_LIST})              # 是用来定义生成的库的名字，以及生成库的类型和生成库需要的源文件 

