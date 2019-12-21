cmake_minimum_required(VERSION 3.8)
project(fakesocks)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

set(SOURCE_FILES main.cpp resolveDNS.cpp resolveDNS.h common.cpp common.h TCPhandler.cpp TCPhandler.h commonHeaders.h eventLoop.cpp eventLoop.h manager.cpp)
add_executable(fakesocks ${SOURCE_FILES})
target_link_libraries(fakesocks encrypt)
