message(STATUS "############ Entering server_src #############")
message(STATUS "This is executable_output_path " ${EXECUTABLE_OUTPUT_PATH})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_ICE_BINARY_DIR})
message(STATUS "This is executable_output_path " ${EXECUTABLE_OUTPUT_PATH})

set(CMAKE_C_COMPILER g++)

include_directories(${CMAKE_ICE_SOURCE_DIR}/util 
    "/usr/local/Ice-3.6.1/include")
link_directories(/usr/local/Ice-3.6.1/lib64/)

set(SRC_LIST 
${CMAKE_ICE_SOURCE_DIR}/util/Printer.cpp
Server.cpp
)
add_executable(server ${SRC_LIST})

#add_executable(server Printer.cpp Server.cpp)
target_link_libraries(server Ice IceUtil)
