#include "HTTPResponse.hpp"

std::string_view HTTPResponse::statusToString(HTTPResponse::StatusCode code)
{
    switch (code)
    {
    case Continue:
        return "Continue";
    case SwitchingProtocols:
        return "SwitchingProtocols";
    case Processing:
        return "Processing";
    case EarlyHints:
        return "EarlyHints";
    case Ok:
        return "OK";
    case Created:
        return "Created";
    case Accepted:
        return "Accepted";
    case NonAuthoritativeInformation:
        return "NonAuthoritativeInformation";
    case NoContent:
        return "NoContent";
    case ResetContent:
        return "ResetContent";
    case PartialContent:
        return "PartialContent";
    case MultiStatus:
        return "MultiStatus";
    case AlreadyReported:
        return "AlreadyReported";
    case IMUsed:
        return "IMUsed";
    case MultipleChoices:
        return "MultipleChoices";
    case MovedPermanently:
        return "MovedPermanently";
    case Found:
        return "Found";
    case SeeOther:
        return "SeeOther";
    case NotModified:
        return "NotModified";
    case UseProxy:
        return "UseProxy";
    case SwitchProxy:
        return "SwitchProxy";
    case TemporaryRedirect:
        return "TemporaryRedirect";
    case PermanentRedirect:
        return "PermanentRedirect";
    case BadRequest:
        return "BadRequest";
    case Unauthorized:
        return "Unauthorized";
    case PaymentRequired:
        return "PaymentRequired";
    case Forbidden:
        return "Forbidden";
    case NotFound:
        return "NotFound";
    case MethodNotAllowed:
        return "MethodNotAllowed";
    case NotAcceptable:
        return "NotAcceptable";
    case ProxyAuthenticationRequired:
        return "ProxyAuthenticationRequired";
    case RequestTimeout:
        return "RequestTimeout";
    case Conflict:
        return "Conflict";
    case Gone:
        return "Gone";
    case LengthRequired:
        return "LengthRequired";
    case PreconditionFailed:
        return "PreconditionFailed";
    case PayloadTooLarge:
        return "PayloadTooLarge";
    case URITooLong:
        return "URITooLong";
    case UnsupportedMediaType:
        return "UnsupportedMediaType";
    case RangeNotSatisfiable:
        return "RangeNotSatisfiable";
    case ExpectationFailed:
        return "ExpectationFailed";
    case ImTeapot:
        return "ImTeapot";
    case MisdirectedRequest:
        return "MisdirectedRequest";
    case UnprocessableEntity:
        return "UnprocessableEntity";
    case Locked:
        return "Locked";
    case FailedDependency:
        return "FailedDependency";
    case UpgradeRequired:
        return "UpgradeRequired";
    case PreconditionRequired:
        return "PreconditionRequired";
    case TooManyRequests:
        return "TooManyRequests";
    case RequestHeaderFieldsTooLarge:
        return "RequestHeaderFieldsTooLarge";
    case UnavailableForLegalReasons:
        return "UnavailableForLegalReasons";
    case InternalServerError:
        return "InternalServerError";
    case NotImplemented:
        return "NotImplemented";
    case BadGateway:
        return "BadGateway";
    case ServiceUnavailable:
        return "ServiceUnavailable";
    case GatewayTimeout:
        return "GatewayTimeout";
    case HTTPVersionNotSupported:
        return "HTTPVersionNotSupported";
    case VariantAlsoNegotiates:
        return "VariantAlsoNegotiates";
    case InsufficientStorage:
        return "InsufficientStorage";
    case LoopDetected:
        return "LoopDetected";
    case NotExtended:
        return "NotExtended";
    case NetworkAuthenticationRequired:
        return "NetworkAuthenticationRequired";
    case Unknown:break;
    }

    return "Unknown";
}

HTTPResponse::HTTPResponse() :
    m_statusCode(Unknown),
    m_stringStatus(),
    m_version(),
    m_header(),
    m_data(),
    m_dataSize()
{

}

HTTPResponse::StatusCode& HTTPResponse::statusCode()
{
    return m_statusCode;
}

HTTPResponse::StatusCode HTTPResponse::statusCode() const
{
    return m_statusCode;
}

std::string_view HTTPResponse::version() const
{
    return m_version;
}

std::string_view& HTTPResponse::version()
{
    return m_version;
}

HTTPHeader HTTPResponse::header() const
{
    return m_header;
}

HTTPHeader& HTTPResponse::header()
{
    return m_header;
}

std::byte* HTTPResponse::data() const
{
    return m_data;
}

std::size_t HTTPResponse::dataSize() const
{
    return m_dataSize;
}

void HTTPResponse::setData(std::byte* data, std::size_t size)
{
    m_data = data;
    m_dataSize = size;
}

std::size_t HTTPResponse::calculateSerializedSize() const
{
    std::size_t result = 0;

    // Adding version
    if (result + m_version.size() + 1 < result)
    {
        throw std::runtime_error("Size is out of range at adding version.");
    }

    // Version
    result += m_version.size();

    // Space
    result += 1;

    auto statusLength = numDigits(m_statusCode);

    if (result + statusLength + 1 < result)
    {
        throw std::runtime_error("Size is out of range at adding decimal status.");
    }

    // Decimal status
    result += statusLength;

    // Space
    result += 1;

    // String status
    auto stringStatus = statusToString(m_statusCode);

    if (result + stringStatus.size() + 2 < result)
    {
        throw std::runtime_error("Size is out of range at adding string status.");
    }

    // String status
    result += stringStatus.size();

    // CRLF
    result += 2;

    // Header and final CRLF
    result += m_header.calculateSerializedSize();

    result += m_dataSize;

    return result;
}

void HTTPResponse::serialize(std::byte* buffer)
{
    buffer = std::copy(
        (const std::byte*) m_version.begin(),
        (const std::byte*) m_version.end(),
        buffer
    );

    *(buffer++) = static_cast<std::byte>(' ');

    buffer += sprintf((char*) buffer, "%d", m_statusCode);

    *(buffer++) = static_cast<std::byte>(' ');

    auto stringStatus = statusToString(m_statusCode);

    buffer = std::copy(
        (const std::byte*) stringStatus.begin(),
        (const std::byte*) stringStatus.end(),
        buffer
    );

    // CRLF
    *(buffer++) = static_cast<std::byte>('\r');
    *(buffer++) = static_cast<std::byte>('\n');

    // Copying header
    m_header.serialize(buffer);

    // Moving
    buffer += m_header.calculateSerializedSize();

    // Copying data
    std::copy(
        m_data,
        m_data + m_dataSize,
        buffer
    );
}

int HTTPResponse::numDigits(std::size_t number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}
