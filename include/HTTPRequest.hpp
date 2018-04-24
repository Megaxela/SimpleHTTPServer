#pragma once


#include <string_view>
#include "HTTPHeader.hpp"

/**
 * @brief Class, that describes http request.
 *
 * Example:
 * ```
 * GET /index.html HTTP/1.1
 * Host: www.example.com
 * ```
 */
class HTTPRequest
{
public:
    /**
     * @brief HTTP methods.
     */
    enum class Method
    {
          None
        , OPTIONS
        , GET
        , HEAD
        , POST
        , PUT
        , PATCH
        , DELETE
        , TRACE
        , CONNECT
    };

    /**
     * @brief Constructor.
     */
    HTTPRequest();

    /**
     * @brief Method for getting pointer to data.
     * @return Pointer to data.
     */
    std::byte* data() const;

    /**
     * @brief Method for getting data size.
     * @return Amount of data in bytes.
     */
    std::size_t dataSize() const;

    /**
     * @brief Method for getting
     * reference to request method.
     * @return Reference to method.
     */
    Method& method();

    /**
     * @brief Method for getting
     * copy of request method.
     * @return Method.
     */
    Method method() const;

    /**
     * @brief Method for setting
     * request method.
     * @param m Method.
     */
    void setMethod(const Method& m);

    /**
     * @brief Method for setting path.
     * @param path Path string view.
     */
    void setUri(const std::string_view& path);

    /**
     * @brief Method for getting path.
     * @return Path.
     */
    std::string_view uri() const;

    /**
     * @brief Method for getting path reference.
     * @return Path reference.
     */
    std::string_view& uri();

    /**
     * @brief Method for setting protocol string.
     * @param version Protocol string view.
     */
    void setVersion(const std::string_view& version);

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
     * @brief Method for calculation serialized
     * size.
     * @return Size of serialized data in bytes.
     */
    std::size_t calculateSerializedSize() const;

    /**
     * @brief Method for parsing bytes.
     * Bytes has to be actual until object
     * instance exists.
     * @param bytes Shared ptr to bytes.
     * @param size Size.
     * @return Parsing status.
     */
    bool parse(std::byte* bytes, std::size_t size);

    /**
     * @brief Static method for parsing
     * string to method. If parsing was
     * unsuccessful, `None` method will
     * be returned.
     * @param s String representation.
     * @return Method type.
     */
    static Method stringToMethod(const std::string_view& s);

    /**
     * @brief Static method for transforming
     * enum class to string.
     * @param m Method enum
     * @return
     */
    static std::string_view methodToString(Method m);


private:
    Method m_method;

    std::string_view m_uri;
    std::string_view m_version;

    HTTPHeader m_header;

    std::byte* m_data;
    std::size_t m_dataSize;
};

