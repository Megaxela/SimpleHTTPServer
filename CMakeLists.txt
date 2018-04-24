cmake_minimum_required(VERSION 3.10)
project(SimpleHTTPServer)

option(SIMPLEHTTP_BUILD_EXAMPLES "Build examples" On)
option(SIMPLEHTTP_BUILD_TESTS    "Build tests"    On)

add_subdirectory(example)
if (${SIMPLEHTTP_BUILD_EXAMPLES})
endif()

if (${SIMPLEHTTP_BUILD_TESTS})
    add_subdirectory(tests)
endif()

if (WIN32)
    add_definitions(-DOS_WINDOWS)
else()
    add_definitions(-DOS_LINUX)
endif()

add_subdirectory(libraries)

set(CMAKE_CXX_STANDARD 17)

add_library(SimpleHTTPServer STATIC
    src/HTTPServer.cpp
    include/HTTPServer.hpp
    src/HTTPHeader.cpp
    include/HTTPHeader.hpp
    src/HTTPRequest.cpp
    include/HTTPRequest.hpp
    src/HTTPResponse.cpp
    include/HTTPResponse.hpp
    src/Tools/SocketTools.cpp
    include/Tools/SocketTools.hpp
    include/Tools/Network.hpp
    src/RESTServer.cpp
    include/RESTServer.hpp
)

target_link_libraries(SimpleHTTPServer
    ALogger
)

target_include_directories(SimpleHTTPServer PUBLIC
    include
)