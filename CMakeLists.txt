cmake_minimum_required(VERSION 3.10)
project(SimpleHTTPServer)

option(SIMPLEHTTP_BUILD_EXAMPLES "Build examples" Off)

if (${SIMPLEHTTP_BUILD_EXAMPLES})
    add_subdirectory(example)
endif()

set(CMAKE_CXX_STANDARD 17)

add_library(SimpleHTTPServer STATIC

        src/HTTPServer.cpp include/HTTPServer.hpp)