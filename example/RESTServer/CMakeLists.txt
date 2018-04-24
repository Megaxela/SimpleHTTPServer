cmake_minimum_required(VERSION 3.10)
project(RESTServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RESTServer
        src/main.cpp
        src/main.cpp)

target_link_libraries(RESTServer
    SimpleHTTPServer
)
if (WIN32)
    add_definitions(-DOS_WINDOWS)
else()
    add_definitions(-DOS_LINUX)
endif()

target_include_directories(RESTServer PRIVATE
    include
)