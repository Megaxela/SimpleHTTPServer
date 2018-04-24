#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <memory>

/**
 * @brief Container for HTTP document.
 */
class HTTPRequest
{
public:

    using HeaderType = std::pair<std::string_view, std::string_view>;

    using HeadersContainer = std::vector<HeaderType>;

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
     * @brief Destructor.
     */
    ~HTTPRequest();

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
     * @brief Method for getting actual data.
     * @return Pointer to actual data.
     */
    std::byte* data() const;

    /**
     * @brief Method for getting data size.
     * @return Data size.
     */
    std::size_t dataSize() const;

    /**
     * @brief Method for setting actual data.
     * @param data Pointer to actual data.
     * @param size Actual data size.
     */
    void setData(std::byte* data, std::size_t size);

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
     * @return Protocl string view.
     */
    std::string_view version() const;

    /**
     * @brief Method for getting protocol reference.
     * @return Protocol string view reference.
     */
    std::string_view& version();

    /**
     * @brief Method for getting number of
     * headers.
     * @return Number of headers.
     */
    HeadersContainer::size_type numberOfHeaders() const;

    /**
     * @brief Method for getting copy of header by index.
     * @param index Header index.
     * @return Header copy.
     */
    HeaderType header(HeadersContainer::size_type index) const;

    /**
     * @brief Method for getting header reference by index.
     * @param index Header index.
     * @return Reference to header.
     */
    HeaderType& header(HeadersContainer::size_type index);

    /**
     * @brief Method for adding new header to request.
     * @param header Header.
     */
    void addHeader(HeaderType header);

    /**
     * @brief Method for insertion new header.
     * @param header New header.
     * @param index Insertion index.
     */
    void insertHeader(HeaderType header, HeadersContainer::size_type index);

    /**
     * @brief Method for removing header by index.
     * @param index Header index.
     */
    void removeHeader(HeadersContainer::size_type index);

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

    /**
     * @brief Method for clearing document.
     */
    void clear();

    /**
     * @brief Method for parsing bytes.
     * Bytes has to be actual until object
     * instance exists.
     * @param bytes Shared ptr to bytes.
     * @param size Size.
     */
    bool parse(std::byte* bytes, std::size_t size);

    /**
     * @brief Method for calculating
     * size of resulting buffer.
     * Can throw std::runtime_error if
     * size if bigger, than std::size_t.
     * @return Required buffer size.
     */
    std::size_t calculateSerializedSize() const;

    /**
     * @brief Method for serializing data into
     * buffer. Buffer size has to be >= than
     * size calculated by `HTTPRequest::calculateSerializedSize`.
     * @param buffer Pointer to buffer.
     */
    void serailize(std::byte* buffer);

private:

    Method m_method;

    std::string_view m_uri;
    std::string_view m_version;

    HeadersContainer m_headers;
};

