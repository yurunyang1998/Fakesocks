cmake_minimum_required(VERSION 3.8)
project(fakesocks)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp resolveDNS.cpp resolveDNS.h common.cpp common.h TCPhandler.cpp TCPhandler.h commonHeaders.h eventLoop.cpp eventLoop.h)
add_executable(fakesocks ${SOURCE_FILES})