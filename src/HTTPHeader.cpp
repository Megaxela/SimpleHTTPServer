#include <map>
#include <CurrentLogger.hpp>
#include "HTTPHeader.hpp"

HTTPHeader::HTTPHeader() :
    m_method(Method::None),
    m_uri(),
    m_version(),
    m_headers()
{

}

HTTPHeader::~HTTPHeader()
{
    m_data = nullptr;
    m_dataSize = 0;
}

HTTPHeader::Method& HTTPHeader::method()
{
    return m_method;
}

HTTPHeader::Method HTTPHeader::method() const
{
    return m_method;
}

void HTTPHeader::setMethod(const HTTPHeader::Method& m)
{
    m_method = m;
}

std::byte* HTTPHeader::data() const
{
    return m_data;
}

std::size_t HTTPHeader::dataSize() const
{
    return m_dataSize;
}

void HTTPHeader::setData(std::byte* data, std::size_t size)
{
    m_data = data;
    m_dataSize = size;
}

void HTTPHeader::setUri(const std::string_view& path)
{
    m_uri = path;
}

std::string_view HTTPHeader::uri() const
{
    return m_uri;
}

std::string_view& HTTPHeader::uri()
{
    return m_uri;
}

void HTTPHeader::setVersion(const std::string_view& version)
{
    m_version = version;
}

std::string_view HTTPHeader::version() const
{
    return m_version;
}

std::string_view& HTTPHeader::version()
{
    return m_version;
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

void HTTPHeader::removeHeader(unsigned long index)
{

}

static const std::map<std::string_view, HTTPHeader::Method> methods = {
    {"OPTIONS", HTTPHeader::Method::OPTIONS},
    {"GET",     HTTPHeader::Method::GET},
    {"HEAD",    HTTPHeader::Method::HEAD},
    {"POST",    HTTPHeader::Method::POST},
    {"PUT",     HTTPHeader::Method::PUT},
    {"PATCH",   HTTPHeader::Method::PATCH},
    {"DELETE",  HTTPHeader::Method::DELETE},
    {"TRACE",   HTTPHeader::Method::TRACE},
    {"CONNECT", HTTPHeader::Method::CONNECT}
};

HTTPHeader::Method HTTPHeader::stringToMethod(const std::string_view& s)
{
    auto result = methods.find(s);

    if (result == methods.end())
    {
        return HTTPHeader::Method::None;
    }

    return result->second;
}

std::string_view HTTPHeader::methodToString(HTTPHeader::Method m)
{
    switch (m)
    {
    case Method::None:      return "None";
    case Method::OPTIONS:   return "OPTIONS";
    case Method::GET:       return "GET";
    case Method::HEAD:      return "HEAD";
    case Method::POST:      return "POST";
    case Method::PUT:       return "PUT";
    case Method::PATCH:     return "PATCH";
    case Method::DELETE:    return "DELETE";
    case Method::TRACE:     return "TRACE";
    case Method::CONNECT:   return "CONNECT";
    }
}

void HTTPHeader::clear()
{
    m_headers.clear();
    m_dataSize = 0;
    m_data = nullptr;
    m_method = Method::None;
}

bool HTTPHeader::parse(std::byte* bytes, std::size_t size)
{
    // Searching for method end
    std::size_t prevIterator = 0;
    std::size_t iterator;

    {
        for (iterator = 0; iterator < size && bytes[iterator] != static_cast<std::byte>(' '); ++iterator);

        // Can't find method end
        if (iterator >= size)
        {
            Warning() << "Can't find space for method.";
            return false;
        }

        std::string_view method(reinterpret_cast<const char*>(bytes + prevIterator), iterator - prevIterator);

        // Trying to parse method to enum
        auto parsedMethod = stringToMethod(method);

        // If parsing was unsuccessful
        if (parsedMethod == Method::None)
        {
            Warning() << "Unknown method \"" << method << "\".";
            return false;
        }

        // Ok, parsedMethod is confirmed.
        iterator += 1;
        prevIterator = iterator;

        // Searching for next space
        for (; iterator < size && bytes[iterator] != static_cast<std::byte>(' '); ++iterator);

        bool hasVersion = true;

        if (iterator >= size)
        {
            // It's OK, if it's CRLF already, cause it can be HTTP/0.9
            iterator = prevIterator;

            for (; iterator < size - 1 &&
                   (bytes[iterator    ] != static_cast<std::byte>(0x0D) ||
                    bytes[iterator + 1] != static_cast<std::byte>(0x0A));
                   ++iterator);

            if (iterator >= size)
            {
                Warning() << "Can't find space for uri.";
                return false;
            }

            hasVersion = false;
        }

        // Receiving uri
        std::string_view parsedURI(reinterpret_cast<const char*>(bytes + prevIterator), iterator - prevIterator);

        std::string_view parsedVersion;

        if (hasVersion)
        {
            // Ok, URI is confirmed there
            iterator += 1;
            prevIterator = iterator;

            // Searching for newline
            for (; iterator < size - 1 &&
                   (bytes[iterator    ] != static_cast<std::byte>(0x0D) ||
                    bytes[iterator + 1] != static_cast<std::byte>(0x0A));
                   ++iterator);

            if (iterator >= size)
            {
                Warning() << "Can't find newline after first line.";
                return false;
            }

            // Receiving version
            parsedVersion = std::string_view(reinterpret_cast<const char*>(bytes + prevIterator), iterator - prevIterator);
        }

        // Ok, version confirmed there
        iterator += 2;
        prevIterator = iterator;

        // Committing values and starting parsing headers
        m_method = parsedMethod;
        m_uri = parsedURI;
        m_version = parsedVersion;
    }

    // Parsing headers
    while (iterator < size)
    {
        // Searching for ": "
        for (; iterator < size - 1 &&
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
            break;
        }
    }

    if (iterator < size)
    {
        m_data = bytes + iterator;
        m_dataSize = size - iterator;
    }

    return true;
}

std::size_t HTTPHeader::calculateSerializedSize() const
{
    std::size_t result = methodToString(m_method).size();

    // Adding space
    ++result;

    // Adding uri
    if (result + m_uri.size() + 1 < result)
    {
        throw std::runtime_error("Size is out of range at adding uri.");
    }

    result += m_uri.size();

    // Adding space
    ++result;

    // Adding version
    if (result + m_version.size() + 2 < result)
    {
        throw std::runtime_error("Size is out of range at adding version.");
    }

    result += m_version.size();

    // Adding new line
    result += 2; // CRLF

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
    if (result + 4 < result)
    {
        throw std::runtime_error("Size is out of range at adding space between headers and data.");
    }

    result += 4;

    // Adding data
    if (result + m_dataSize < result)
    {
        throw std::runtime_error("Size is out of range at adding data.");
    }

    result += m_dataSize;

    return result;
}

void HTTPHeader::serailize(std::byte* buffer)
{

}
