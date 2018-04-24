
#include <gtest/gtest.h>
#include <HTTPHeader.hpp>

TEST(HTTPHeader, ParsingEndSize)
{
    std::size_t size = 24 + 91 + 19 + 19;
    auto* value =
        (std::byte *)
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"}
    };

    HTTPHeader httpHeader;

    ASSERT_TRUE(httpHeader.parse(value, size));

    for (HTTPHeader::HeadersContainer::size_type i = 0;
         i < httpHeader.numberOfHeaders();
         ++i)
    {
        auto header = httpHeader.header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }
}

TEST(HTTPHeader, ParsingEndTLDR)
{
    std::size_t size = 24 + 91 + 19 + 19 + 2;
    auto* value =
        (std::byte *)
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"}
    };

    HTTPHeader httpHeader;

    ASSERT_TRUE(httpHeader.parse(value, size));

    for (HTTPHeader::HeadersContainer::size_type i = 0;
         i < httpHeader.numberOfHeaders();
         ++i)
    {
        auto header = httpHeader.header(i);

        ASSERT_EQ(header.first,  intendedHeaders[i].first );
        ASSERT_EQ(header.second, intendedHeaders[i].second);
    }
}

TEST(HTTPHeader, Serialization)
{
    constexpr std::size_t size = 24 + 91 + 19 + 19 + 2;
    auto* value =
        (std::byte *)
            "Host: ru.wikipedia.org\r\n"
            "User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n"
            "Accept: text/html\r\n"
            "Connection: close\r\n"
            "\r\n";

    std::vector<HTTPHeader::HeaderType> intendedHeaders = {
        {"Host", "ru.wikipedia.org"},
        {"User-Agent", "Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5"},
        {"Accept", "text/html"},
        {"Connection", "close"}
    };

    HTTPHeader httpHeader;

    for (auto&& element : intendedHeaders)
    {
        httpHeader.addHeader(element);
    }

    ASSERT_EQ(httpHeader.calculateSerializedSize(), size);

    std::byte buffer[size];

    httpHeader.serialize(buffer);

    // Validating result
    for (std::size_t i = 0; i < size; ++i)
    {
        ASSERT_EQ(value[i], buffer[i]);
    }
}
