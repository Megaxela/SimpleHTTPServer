#pragma once

#include <vector>
#include <string>
#include <cstddef>
#include <memory>

/**
 * @brief Container for HTTP header.
 */
class HTTPHeader
{
public:

    using HeaderType = std::pair<std::string_view, std::string_view>;

    using HeadersContainer = std::vector<HeaderType>;


    /**
     * @brief Constructor.
     */
    HTTPHeader();

    /**
     * @brief Destructor.
     */
    ~HTTPHeader() = default;

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
    bool parse(std::byte* bytes, std::size_t size, std::size_t* finish=nullptr);

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
    void serialize(std::byte* buffer);

private:

    HeadersContainer m_headers;
};

