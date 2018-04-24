
#include <gtest/gtest.h>
#include <HTTPHeader.hpp>
#include <HTTPRequest.hpp>

TEST(HTTPRequest, Creation)
{
    HTTPRequest request;

    ASSERT_EQ(request.method(),         HTTPRequest::Method::None);
    ASSERT_EQ(request.data(),           nullptr);
    ASSERT_EQ(request.dataSize(),       0);
    ASSERT_EQ(request.uri().size(),     0);
    ASSERT_EQ(request.version().size(), 0);
}

TEST(HTTPRequest, ParsingFavicon)
{
    std::size_t size = 435;
    auto* value =
        (std::byte *)
            "GET /favicon.ico HTTP/1.1\r\n"
            "Host: 127.0.0.1:1212\r\n"
            "Connection: keep-alive\r\n"
            "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.117 Safari/537.36\r\n"
            "Accept: image/webp,image/apng,image/*,*/*;q=0.8\r\n"
            "DNT: 1\r\n"
            "Referer: http://127.0.0.1:1212/\r\n"
            "Accept-Encoding: gzip, deflate, br\r\n"
            "Accept-Language: en-US,en;q=0.9,ru;q=0.8\r\n"
            "X-Compress: null\r\n"
            "\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "127.0.0.1:1212"},
        {"Connection", "keep-alive"},
        {"User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.117 Safari/537.36"},
        {"Accept", "image/webp,image/apng,image/*,*/*;q=0.8"},
        {"DNT", "1"},
        {"Referer", "http://127.0.0.1:1212/"},
        {"Accept-Encoding", "gzip, deflate, br"},
        {"Accept-Language", "en-US,en;q=0.9,ru;q=0.8"},
        {"X-Compress", "null"}
    };

    HTTPRequest request;

    ASSERT_TRUE(request.parse(value, size));
    ASSERT_EQ(request.method(), HTTPRequest::Method::GET);
    ASSERT_EQ(request.uri(), "/favicon.ico");
    ASSERT_EQ(request.version(), "HTTP/1.1");

    for (HTTPHeader::HeadersContainer::size_type i = 0;
         i < request.header().numberOfHeaders();
         ++i)
    {
        auto header = request.header().header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }
}

TEST(HTTPRequest, ParsingCommon)
{
    std::size_t size = 25 + 24 + 91 + 19 + 19 + 2;
    auto* value =
        (std::byte*)
            "GET /wiki/page HTTP/1.1\r\n"
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n"
            "\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"},
    };

    HTTPRequest request;

    ASSERT_EQ(request.parse(value, size), true);

    ASSERT_EQ(request.method(),  HTTPRequest::Method::GET);
    ASSERT_EQ(request.uri(),     "/wiki/page");
    ASSERT_EQ(request.version(), "HTTP/1.1");

    for (HTTPHeader::HeadersContainer::size_type i = 0;
        i < request.header().numberOfHeaders();
        ++i)
    {
        auto header = request.header().header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }
}

TEST(HTTPRequest, ParsingOld)
{
    std::size_t size = 16 + 24 + 91 + 19 + 19 + 2;
    auto* value =
        (std::byte*)
            "GET /wiki/page\r\n"
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n"
            "\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"},
    };

    HTTPRequest request;

    ASSERT_EQ(request.parse(value, size), true);

    ASSERT_EQ(request.method(),           HTTPRequest::Method::GET);
    ASSERT_EQ(request.uri(),              "/wiki/page");
    ASSERT_EQ(request.version().size(),   0);

    for (HTTPHeader::HeadersContainer::size_type i = 0;
        i < request.header().numberOfHeaders();
        ++i)
    {
        auto header = request.header().header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }
}

TEST(HTTPRequest, ParsingWithData)
{
    std::size_t size = 16 + 24 + 91 + + 19 + 19 + 2 + 8;
    auto* value =
        (std::byte*)
            "GET /wiki/page\r\n"
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "SOMEDATA";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"},
    };

    HTTPRequest request;

    ASSERT_EQ(request.parse(value, size), true);

    ASSERT_EQ(request.method(),           HTTPRequest::Method::GET);
    ASSERT_EQ(request.uri(),              "/wiki/page");
    ASSERT_EQ(request.version().size(),   0);

    for (HTTPHeader::HeadersContainer::size_type i = 0;
         i < request.header().numberOfHeaders();
         ++i)
    {
        auto header = request.header().header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }

    ASSERT_EQ(request.dataSize(), 8);

    auto* dataValue = (std::byte*) "SOMEDATA";

    for (std::size_t i = 0; i < request.dataSize(); ++i)
    {
        ASSERT_EQ(request.data()[i], dataValue[i]);
    }
}