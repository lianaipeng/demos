cmake_minimum_required(VERSION 2.8)

project(CMAKE_ICE)
message(STATUS "this is binary dir" ${CMAKE_ICE_BINARY_DIR})
message(STATUS "this is source dir" ${CMAKE_ICE_SOURCE_DIR})

ADD_SUBDIRECTORY(server_src)
ADD_SUBDIRECTORY(client_src)
