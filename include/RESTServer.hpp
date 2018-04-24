#pragma once

#include "HTTPServer.hpp"
#include "nlohmann/json.hpp"

/**
 * @brief Sample HTTP REST server.
 */
class RESTServer : public HTTPServer
{
public:

    enum class ErrorCode
    {
          Ok               = 0
        , UnknownCommand   = 1
        , InvalidArguments = 2
        , ExceptionCaught  = 3
        , InvalidMethod    = 4
    };

    using Arguments = std::map<std::string_view, std::string_view>;
    using ProcessorFunction = std::function<nlohmann::json(Arguments, std::byte*, std::size_t)>;
    using ErrorProcessorFunction = std::function<nlohmann::json(ErrorCode, std::string)>;

    /**
     * @brief Constructor.
     */
    RESTServer();

    /**
     * @brief Method for adding REST command processor.
     * @param method Request method.
     * @param key Command key. Example: "/api/action"
     * @param function Command processor function.
     */
    void addProcessor(
        HTTPRequest::Method method,
        std::string key,
        ProcessorFunction function
    );

    /**
     * @brief Method for setting error processor.
     * This processor is called on any error.
     * @param function Processor function.
     */
    void setErrorProcessor(ErrorProcessorFunction function);

protected:
    /**
     * @brief Overridden request processing, that
     * parses uri, search for processor by method and
     * forming json response.
     * @param request Request object.
     * @return Response object.
     */
    HTTPResponse proceedRequest(HTTPRequest request) override;

private:
    nlohmann::json proceedREST(HTTPRequest request);

    /**
     * @brief Default error processor.
     * @param error Error code.
     * @param info Error info.
     * @return Formed error json.
     */
    static nlohmann::json defaultErrorProcessor(ErrorCode error, std::string info);

    /**
     * @brief Method for parsing uri to components.
     * @param uri Base URI.
     * @param outPath
     * @param arguments
     * @return
     */
    bool splitUri(
        const std::string_view& uri,
        std::string_view& outPath,
        Arguments& arguments
    );

    std::map<
        std::string,
        std::map<
            HTTPRequest::Method,
            ProcessorFunction
        >
    > m_processors;
    ErrorProcessorFunction m_errorProcessor;

    std::string m_dataBuffer;
};

