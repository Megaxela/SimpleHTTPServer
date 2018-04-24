cmake_minimum_required(VERSION 3.10)
project(SimpleHTTPServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(SimpleHTTPServer
        srcmain.cpp
)

target_link_libraries(SimpleHTTPServer
    SimpleHTTPServer
)