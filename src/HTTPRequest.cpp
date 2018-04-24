#include <map>
#include <CurrentLogger.hpp>
#include "HTTPRequest.hpp"

static const std::map<std::string_view, HTTPRequest::Method> methods = {
    {"OPTIONS", HTTPRequest::Method::OPTIONS},
    {"GET",     HTTPRequest::Method::GET},
    {"HEAD",    HTTPRequest::Method::HEAD},
    {"POST",    HTTPRequest::Method::POST},
    {"PUT",     HTTPRequest::Method::PUT},
    {"PATCH",   HTTPRequest::Method::PATCH},
    {"DELETE",  HTTPRequest::Method::DELETE},
    {"TRACE",   HTTPRequest::Method::TRACE},
    {"CONNECT", HTTPRequest::Method::CONNECT}
};

HTTPRequest::Method HTTPRequest::stringToMethod(const std::string_view& s)
{
    auto result = methods.find(s);

    if (result == methods.end())
    {
        return HTTPRequest::Method::None;
    }

    return result->second;
}

std::string_view HTTPRequest::methodToString(HTTPRequest::Method m)
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

HTTPRequest::HTTPRequest() :
    m_method(Method::None),
    m_uri(),
    m_version(),
    m_header(),
    m_data(nullptr),
    m_dataSize()
{

}

HTTPHeader HTTPRequest::header() const
{
    return m_header;
}

HTTPHeader& HTTPRequest::header()
{
    return m_header;
}

HTTPRequest::Method& HTTPRequest::method()
{
    return m_method;
}

HTTPRequest::Method HTTPRequest::method() const
{
    return m_method;
}

void HTTPRequest::setMethod(const HTTPRequest::Method& m)
{
    m_method = m;
}

void HTTPRequest::setUri(const std::string_view& path)
{
    m_uri = path;
}

std::string_view HTTPRequest::uri() const
{
    return m_uri;
}

std::string_view& HTTPRequest::uri()
{
    return m_uri;
}

void HTTPRequest::setVersion(const std::string_view& version)
{
    m_version = version;
}

std::string_view HTTPRequest::version() const
{
    return m_version;
}

std::string_view& HTTPRequest::version()
{
    return m_version;
}

std::byte* HTTPRequest::data() const
{
    return m_data;
}

std::size_t HTTPRequest::dataSize() const
{
    return m_dataSize;
}

std::size_t HTTPRequest::calculateSerializedSize() const
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

    return result + m_header.calculateSerializedSize();
}

bool HTTPRequest::parse(std::byte* bytes, std::size_t size)
{
    m_header.clear();
    // Searching for method end
    std::size_t prevIterator = 0;
    std::size_t iterator;

    // Skipping initial spaces
    for (iterator = 0;
         iterator < size && bytes[iterator] != static_cast<std::byte>(' ');
         ++iterator);

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
    for (; iterator < size &&
           bytes[iterator] != static_cast<std::byte>(' ') &&
           (bytes[iterator    ] != static_cast<std::byte>(0x0D) ||
            bytes[iterator + 1] != static_cast<std::byte>(0x0A));
           ++iterator);

    bool hasVersion = (bytes[iterator    ] != static_cast<std::byte>(0x0D) ||
                       bytes[iterator + 1] != static_cast<std::byte>(0x0A));

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

    std::size_t resultIterator;

    if (!m_header.parse(bytes + iterator, size - iterator, &resultIterator))
    {
        return false;
    }

    resultIterator += iterator;

    m_data = bytes + resultIterator;
    m_dataSize = size - resultIterator;

    return true;
}
