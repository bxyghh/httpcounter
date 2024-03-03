cmake_minimum_required(VERSION 3.0)

project(httpcounter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror -O2")

add_executable(httpcounter
        main.cpp
        TcpServer.cpp TcpServer.h
        HttpServer.cpp HttpServer.h
        HttpCounter.cpp HttpCounter.h
        MemoryCounter.h)

target_link_libraries(httpcounter pthread)
