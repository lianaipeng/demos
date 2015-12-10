message(STATUS "############ Entering client_src #############")

message(STATUS "this is executable_output_path " ${EXECUTABLE_OUTPUT_PATH})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_ICE_BINARY_DIR})
message(STATUS "this is executable_output_path " ${EXECUTABLE_OUTPUT_PATH})


set(CMAKE_C_COMPILER g++)

include_directories(${CMAKE_ICE_SOURCE_DIR}/util 
    "/usr/local/Ice-3.6.1/include")
link_directories(/usr/local/Ice-3.6.1/lib64/)

set(SRC_LIST ${CMAKE_ICE_SOURCE_DIR}/util/Printer.cpp
Client.cpp
)

add_executable(client ${SRC_LIST})
target_link_libraries(client Ice IceUtil)

