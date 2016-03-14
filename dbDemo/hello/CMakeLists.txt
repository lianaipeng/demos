cmake_minimum_required(VERSION 2.8)
project(forum)

#find_package(Boost)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

SET(CMAKE_C_COMPILER /opt/rh/devtoolset-2/root/usr/bin/gcc)
SET(CMAKE_CXX_COMPILER /opt/rh/devtoolset-2/root/usr/bin/g++)

#打印些东西
message("PROJECT_SOURCE_DIR " "${PROJECT_SOURCE_DIR}")
#SET(PROJECT_ROOT_PATH "${PROJECT_SOURCE_DIR}")
#message("PROJECT_ROOT_PATH " "${PROJECT_ROOT_PATH}")


link_directories(
#"/home/lianaipeng/download/Ice-3.6.1/lib64/c++11/"
#"/usr/local/Ice-3.6.1/lib64/c++11/"
#"/usr/local/Ice-3.6.1/lib64/"
        "/home/lianaipeng/download/ice-3.6.1/cpp/lib64"
        "${PROJECT_SOURCE_DIR}/../third_party/lib_acl/lib/linux64"
        "/usr/lib64/"
        "/usr/local/lib64/"
        )

# 头文件
include_directories(
        "${PROJECT_SOURCE_DIR}/interface/"
        "${PROJECT_SOURCE_DIR}/src/"
#"/usr/local/Ice-3.6.1/include/"
#"/usr/local/Ice-3.6.1/include/"
        "/home/lianaipeng/download/ice-3.6.1/cpp/include"
#"/home/lianaipeng/download/Ice-3.6.1/include/"
        "${PROJECT_SOURCE_DIR}/../common/"
        "${PROJECT_SOURCE_DIR}/../third_party/lib_acl/include/"
        )

# 源文件
set(FORUM_SERVER_SRC
        "${PROJECT_SOURCE_DIR}/../common/log.cpp" 
        "${PROJECT_SOURCE_DIR}/../common/mysql_pool.cpp"
        ../interface/Forum.cpp  
        common.cpp ForumI.cpp ForumDao.cpp 
        main.cpp  
   )

# 目标文件
add_executable(forum_server
        ${FORUM_SERVER_SRC})

target_link_libraries(forum_server
        pthread
        _acl_cpp
        _protocol
        _acl
        mysqlcppconn
        )
