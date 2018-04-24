#pragma once

#include <string_view>
#include "HTTPHeader.hpp"

/**
 * @brief Class, that describes http response.
 *
 * Example:
 * ```
 * HTTP/1.1 200 OK
 * Date: Mon, 23 May 2005 22:38:34 GMT
 * Content-Type: text/html; charset=UTF-8
 * Content-Encoding: UTF-8
 * Content-Length: 138
 * Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT
 * Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)
 * ETag: "3f80f-1b6-3e1cb03b"
 * Accept-Ranges: bytes
 * Connection: close
 *
 * <html>
 * <head>
 *   <title>An Example Page</title>
 * </head>
 * <body>
 *   Hello World, this is a very simple HTML document.
 * </body>
 * </html>
 * ```
 */
class HTTPResponse
{
public:

    enum StatusCode
    {
        Unknown = 0

        // 1xx Informational responses
        , Continue           = 100
        , SwitchingProtocols = 101
        , Processing         = 102
        , EarlyHints         = 103

        // 2xx Success
        , Ok                          = 200
        , Created                     = 201
        , Accepted                    = 202
        , NonAuthoritativeInformation = 203
        , NoContent                   = 204
        , ResetContent                = 205
        , PartialContent              = 206
        , MultiStatus                 = 207
        , AlreadyReported             = 208
        , IMUsed                      = 226

        // 3xx Redirection
        , MultipleChoices   = 300
        , MovedPermanently  = 301
        , Found             = 302
        , SeeOther          = 303
        , NotModified       = 304
        , UseProxy          = 305
        , SwitchProxy       = 306
        , TemporaryRedirect = 307
        , PermanentRedirect = 308

        // 4xx Client errors
        , BadRequest                  = 400
        , Unauthorized                = 401
        , PaymentRequired             = 402
        , Forbidden                   = 403
        , NotFound                    = 404
        , MethodNotAllowed            = 405
        , NotAcceptable               = 406
        , ProxyAuthenticationRequired = 407
        , RequestTimeout              = 408
        , Conflict                    = 409
        , Gone                        = 410
        , LengthRequired              = 411
        , PreconditionFailed          = 412
        , PayloadTooLarge             = 413
        , URITooLong                  = 414
        , UnsupportedMediaType        = 415
        , RangeNotSatisfiable         = 416
        , ExpectationFailed           = 417
        , ImTeapot                    = 418 //???
        , MisdirectedRequest          = 421
        , UnprocessableEntity         = 422
        , Locked                      = 423
        , FailedDependency            = 424
        , UpgradeRequired             = 426
        , PreconditionRequired        = 428
        , TooManyRequests             = 429
        , RequestHeaderFieldsTooLarge = 431
        , UnavailableForLegalReasons  = 451

        // 5xx Server errors
        , InternalServerError           = 500
        , NotImplemented                = 501
        , BadGateway                    = 502
        , ServiceUnavailable            = 503
        , GatewayTimeout                = 504
        , HTTPVersionNotSupported       = 505
        , VariantAlsoNegotiates         = 506
        , InsufficientStorage           = 507
        , LoopDetected                  = 508
        , NotExtended                   = 510
        , NetworkAuthenticationRequired = 511
    };

    /**
     * @brief Method for receiving string name of status code.
     * @param code Code.
     * @return String view of status code.
     */
    static std::string_view statusToString(StatusCode code);

    /**
     * @brief Constructor.
     */
    HTTPResponse();

    /**
     * @brief Method for getting response status
     * code reference.
     * @return Reference to status code.
     */
    StatusCode& statusCode();

    /**
     * @brief Method for getting response status code.
     * @return Status code.
     */
    StatusCode statusCode() const;

    /**
     * @brief Method for getting protocol.
     * @return Protocol string view.
     */
    std::string_view version() const;

    /**
     * @brief Method for getting protocol reference.
     * @return Protocol string view reference.
     */
    std::string_view& version();

    /**
     * @brief Method for getting header.
     * @return Header.
     */
    HTTPHeader header() const;

    /**
     * @brief Method for getting header reference.
     * @return Header reference.
     */
    HTTPHeader& header();

    /**
     * @brief Method for getting pointer to data.
     * @return Pointer to data.
     */
    std::byte* data() const;

    /**
     * @brief Method for getting size of data.
     * @return Data size in bytes.
     */
    std::size_t dataSize() const;

    /**
     * @brief Method for setting pointer to data.
     * @param data Pointer to data.
     * @param size Data size in bytes.
     */
    void setData(std::byte* data, std::size_t size);

    /**
     * @brief Method for calculation serialized
     * size.
     * @return Size of serialized data in bytes.
     */
    std::size_t calculateSerializedSize() const;

    /**
     * @brief Method for serializing data into
     * byffer. Buffer has to be >= than
     * size, calculated by `HTTPResponse::calculateSerializedSize`.
     * @param buffer Pointer to buffer.
     */
    void serialize(std::byte* buffer);

private:

    static int numDigits(std::size_t number);

    StatusCode m_statusCode;
    std::string_view m_stringStatus;
    std::string_view m_version;

    HTTPHeader m_header;

    std::byte* m_data;
    std::size_t m_dataSize;
};

