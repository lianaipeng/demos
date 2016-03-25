cmake_minimum_required(VERSION 2.6)
project(forum)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(CMAKE_C_COMPILER /opt/rh/devtoolset-2/root/usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /opt/rh/devtoolset-2/root/usr/bin/g++)

#打印些东西
#message("PROJECT_SOURCE_DIR " "${PROJECT_SOURCE_DIR}/")

# 生成ice文件
add_custom_command( OUTPUT "${PROJECT_SOURCE_DIR}/interface/Forum.cpp" COMMAND
        slice2cpp "${PROJECT_SOURCE_DIR}/interface/Forum.ice" --underscore
        --output-dir "${PROJECT_SOURCE_DIR}/interface/" DEPENDS
        "${PROJECT_SOURCE_DIR}/interface/Forum.ice")

# 链接库
link_directories(
    #"/usr/local/Ice-3.6.1/lib64"
    "/usr/local/Ice-3.6.1/lib64/c++11/"
)

# 头文件
include_directories(
"${PROJECT_SOURCE_DIR}/interface/"
"${PROJECT_SOURCE_DIR}/src/"
"/usr/local/Ice-3.6.1/include"
)

# 服务器端程序
add_subdirectory(src)
# 测试程序
add_subdirectory(test)
