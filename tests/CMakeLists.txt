project(SimpleHTTPServerTests)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

add_executable(SimpleHTTPServerTests
        main.cpp
        HTTPRequest.cpp HTTPHeader.cpp HTTPResponse.cpp)

target_link_libraries(SimpleHTTPServerTests
    gtest
    SimpleHTTPServer
)