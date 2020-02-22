cmake_minimum_required(VERSION 3.8)
project(fakesocks)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

set(SOURCE_FILES main.cpp tcp_server.cpp tcp_server.h tcp_connection.cpp tcp_connection.h)
add_executable(fakesocks ${SOURCE_FILES})

target_link_libraries(fakesocks  pthread boost_system)
add_compile_options(dynamic)

