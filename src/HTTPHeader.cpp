#include <map>
#include <CurrentLogger.hpp>
#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader() :
    m_headers()
{

}

unsigned long HTTPHeader::numberOfHeaders() const
{
    return m_headers.size();
}

HTTPHeader::HeaderType HTTPHeader::header(HeadersContainer::size_type index) const
{
    return m_headers[index];
}

HTTPHeader::HeaderType& HTTPHeader::header(HeadersContainer::size_type index)
{
    return m_headers[index];
}

void HTTPHeader::addHeader(HTTPHeader::HeaderType header)
{
    m_headers.push_back(std::move(header));
}

void HTTPHeader::insertHeader(HTTPHeader::HeaderType header, HeadersContainer::size_type index)
{
    m_headers.insert(m_headers.begin() + index, std::move(header));
}

void HTTPHeader::removeHeader(HeadersContainer::size_type index)
{
    m_headers.erase(m_headers.begin() + index);
}

void HTTPHeader::clear()
{
    m_headers.clear();
}

bool HTTPHeader::parse(std::byte* bytes, std::size_t size, std::size_t* finish)
{
    // Searching for method end
    std::size_t prevIterator = 0;
    std::size_t iterator = 0;

    // Parsing headers
    while (iterator < size)
    {
//        std::cout << "I: " << iterator << ", S: " << size << std::endl;
        // Searching for ": "
        for (;
             iterator < size - 1 &&
               (bytes[iterator    ] != static_cast<std::byte>(':') ||
                bytes[iterator + 1] != static_cast<std::byte>(' '));
               ++iterator);

        if (iterator >= size)
        {
            Warning() << "Can't find header splitter.";
            return false;
        }

        // Receiving name
        std::string_view parsedName(reinterpret_cast<const char*>(bytes + prevIterator), iterator - prevIterator);

        // Ok, name parsed
        iterator += 2; // ": "
        prevIterator = iterator;

        // Searching for newline
        for (; iterator < size - 1 &&
               (bytes[iterator    ] != static_cast<std::byte>(0x0D) ||
                bytes[iterator + 1] != static_cast<std::byte>(0x0A));
               ++iterator);

        if (iterator >= size)
        {
            Warning() << "Can't find newline after header line.";
            return false;
        }

        // Receiving version
        std::string_view parsedValue(reinterpret_cast<const char*>(bytes + prevIterator), iterator - prevIterator);

        // Ok, version confirmed there
        iterator += 2;
        prevIterator = iterator;

        // Committing data
        m_headers.emplace_back(parsedName, parsedValue);

        // Is it headers finish?
        if (iterator + 2 <= size &&
            (bytes[iterator    ] == static_cast<std::byte>(0x0D) &&
             bytes[iterator + 1] == static_cast<std::byte>(0x0A)))
        {
            iterator += 2;
            break;
        }
    }

    if (finish)
    {
        *finish = iterator;
    }

    return true;
}

std::size_t HTTPHeader::calculateSerializedSize() const
{
    std::size_t result = 0;

    // Adding headers
    for (auto header : m_headers)
    {
        // Adding name
        if (result + header.first.size() + 2 < result)
        {
            throw std::runtime_error("Size is out of range at adding header name.");
        }

        result += header.first.size();

        // Adding ": "
        result += 2;

        // Adding value
        if (result + header.second.size() + 2 < result)
        {
            throw std::runtime_error("Size is out of range at adding header value.");
        }

        result += header.second.size();

        // Adding new line
        result += 2; // CRLF
    }

    // Adding 2 new lines
    if (result + 2 < result)
    {
        throw std::runtime_error("Size is out of range at adding space between headers and data.");
    }

    result += 2;

    return result;
}

void HTTPHeader::serialize(std::byte* buffer)
{
    for (auto&& header : m_headers)
    {
        // Name
        buffer = std::copy(
            (const std::byte*) header.first.begin(),
            (const std::byte*) header.first.end(),
            buffer
        );

        // ": "
        *(buffer++) = static_cast<std::byte>(':');
        *(buffer++) = static_cast<std::byte>(' ');

        // Value
        buffer = std::copy(
            (const std::byte*) header.second.begin(),
            (const std::byte*) header.second.end(),
            buffer
        );

        // CRLF
        *(buffer++) = static_cast<std::byte>('\r');
        *(buffer++) = static_cast<std::byte>('\n');
    }

    // CRLF
    *(buffer++) = static_cast<std::byte>('\r');
     *buffer    = static_cast<std::byte>('\n');
}
