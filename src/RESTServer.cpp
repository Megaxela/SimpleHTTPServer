#include <CurrentLogger.hpp>
#include "RESTServer.hpp"

RESTServer::RESTServer() :
    m_processors(),
    m_errorProcessor(&RESTServer::defaultErrorProcessor),
    m_dataBuffer()
{

}

HTTPResponse RESTServer::proceedRequest(HTTPRequest request)
{
    std::string_view mainUri;
    Arguments args;

    nlohmann::json result = proceedREST(std::move(request));

    m_dataBuffer = result.dump();

    HTTPResponse response;
    response.version() = "HTTP/1.1";
    response.statusCode() = HTTPResponse::StatusCode::Ok;
    response.setData(
        (std::byte*) m_dataBuffer.c_str(),
        m_dataBuffer.length()
    );

    response.header().addHeader({"Content-Type", "application/json"});

    return response;
}

nlohmann::json RESTServer::proceedREST(HTTPRequest request)
{

    std::string_view mainUri;
    Arguments args;

    if (!splitUri(request.uri(), mainUri, args))
    {
        Error() << "Can't split uri to args.";
        return m_errorProcessor(ErrorCode::InvalidArguments, "Can't parse URI.");
    }

    // Searching for command key
    auto commandKey = m_processors.find(std::string(mainUri));

    if (commandKey == m_processors.end())
    {
        Error() << "Unknown command \"" << mainUri << "\".";
        return m_errorProcessor(
            ErrorCode::UnknownCommand,
            std::string("Unknown command \"") + std::string(mainUri)+ "\"."
        );
    }

    // Searching for method in command
    auto methodKey = commandKey->second.find(request.method());

    if (methodKey == commandKey->second.end())
    {
        Error()
            << "Unknown method \""
            << HTTPRequest::methodToString(request.method())
            << "\" for command \""
            << mainUri
            << "\".";

        return m_errorProcessor(
            ErrorCode::InvalidMethod,
            std::string("Unknown method \"") +
                std::string(HTTPRequest::methodToString(request.method())) +
                "\" for command \"" +
                std::string(mainUri) +
                "\"."
            );
    }

    Info() << "Requested command \"" << mainUri << "\".";

    try
    {
        return methodKey->second(args, request.data(), request.dataSize());
    }
    catch (std::exception& exception)
    {
        Error() << "Received exception. What: " << exception.what();
        return m_errorProcessor(
            ErrorCode::ExceptionCaught,
            exception.what()
        );
    }
}

void RESTServer::addProcessor(HTTPRequest::Method method, std::string key, RESTServer::ProcessorFunction function)
{
    auto processor = m_processors.find(key);

    if (processor == m_processors.end())
    {
        processor = m_processors.insert_or_assign(
            key,
            std::map<
                HTTPRequest::Method,
                ProcessorFunction
            >()
        ).first;
    }

    processor->second[method] = std::move(function);
}

void RESTServer::setErrorProcessor(RESTServer::ErrorProcessorFunction function)
{
    m_errorProcessor = std::move(function);
}

nlohmann::json RESTServer::defaultErrorProcessor(RESTServer::ErrorCode error, std::string info)
{
    nlohmann::json result;
    result["error_code"] = (int) error;
    result["error_string"] = info;

    return result;
}

bool RESTServer::splitUri(const std::string_view& uri, std::string_view& outPath, RESTServer::Arguments& arguments)
{
    auto target = uri.find('?');

    if (target == std::string_view::npos)
    {
        outPath = uri;
        return true;
    }

    auto start = uri.find('/');
    if (start != 1)
    {
        start = 0;
    }

    outPath = uri.substr(start, target - start);

    auto separator = uri.find('=', target);

    if (separator == std::string_view::npos)
    {
        // Search finished. There is no value for argument
        return true;
    }

    auto nextTarget = uri.find('&', separator + 1);

    auto finish = nextTarget;

    if (finish == std::string_view::npos)
    {
        finish = uri.size();
    }

    arguments[uri.substr(target + 1, separator - target - 1)] =
        uri.substr(separator + 1, finish - separator - 1);

    target = uri.find('&', target);

    while (target != std::string_view::npos)
    {
        auto separator = uri.find('=', target);

        if (separator == std::string_view::npos)
        {
            // Search finished. There is no value for argument
            return true;
        }

        auto nextTarget = uri.find('&', separator + 1);

        finish = nextTarget;

        if (finish == std::string_view::npos)
        {
            finish = uri.size();
        }

        arguments[uri.substr(target + 1, separator - target - 1)] =
            uri.substr(separator + 1, finish - separator - 1);

        target = nextTarget;
    }

    return true;
}
