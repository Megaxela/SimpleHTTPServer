#include <gtest/gtest.h>
#include <HTTPResponse.hpp>

TEST(HTTPResponse, Common)
{
    HTTPResponse response;

    ASSERT_EQ(response.statusCode(),     0);
    ASSERT_EQ(response.dataSize(),       0);
    ASSERT_EQ(response.data(),           nullptr);
    ASSERT_EQ(response.version().size(), 0);
}

TEST(HTTPResponse, Serializing)
{
    constexpr std::size_t size = 440;

    auto* value =
        (std::byte *)
            "HTTP/1.1 200 OK\r\n"
            "Date: Mon, 23 May 2005 22:38:34 GMT\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Content-Encoding: UTF-8\r\n"
            "Content-Length: 138\r\n"
            "Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\r\n"
            "Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
            "ETag: \"3f80f-1b6-3e1cb03b\"\r\n"
            "Accept-Ranges: bytes\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html>\r\n"
            "<head>\r\n"
            "  <title>An Example Page</title>\r\n"
            "</head>\r\n"
            "<body>\r\n"
            "  Hello World, this is a very simple HTML document.\r\n"
            "</body>\r\n"
            "</html>";

    std::byte* data = value + 304;
    std::size_t dataLength = 136;

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Date",             "Mon, 23 May 2005 22:38:34 GMT"},
        {"Content-Type",     "text/html; charset=UTF-8"},
        {"Content-Encoding", "UTF-8"},
        {"Content-Length",   "138"},
        {"Last-Modified",    "Wed, 08 Jan 2003 23:11:55 GMT"},
        {"Server",           "Apache/1.3.3.7 (Unix) (Red-Hat/Linux)"},
        {"ETag",             "\"3f80f-1b6-3e1cb03b\""},
        {"Accept-Ranges",    "bytes"},
        {"Connection",       "close"},
    };

    HTTPResponse response;

    response.version() = "HTTP/1.1";
    response.statusCode() = HTTPResponse::StatusCode::Ok;

    for (auto&& element : intendedHeaders)
    {
        response.header().addHeader(element);
    }

    response.setData(data, dataLength);

    ASSERT_EQ(response.calculateSerializedSize(), size);

    std::byte buffer[size];

    response.serialize(buffer);

    for (std::size_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(value[i], buffer[i]);
    }
}