
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <HTTPServer.hpp>
#include <Loggers/BasicLogger.hpp>
#include <CurrentLogger.hpp>
#include <RESTServer.hpp>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Wrong usage. Usage: " << std::endl;
        if (argc > 0)
        {
            std::cerr << "    " << argv[0] << " <port>" << std::endl;
        }

        return -1;
    }

    auto port = std::atoi(argv[1]);

    if (port == 0)
    {
        std::cerr << "Wrong port value." << std::endl;
        return -2;
    }

    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    RESTServer server;

    server.addProcessor(
        HTTPRequest::Method::GET,
        "/api/version",
        [](RESTServer::Arguments args, std::byte* data, std::size_t s) -> nlohmann::json
        {
            nlohmann::json result;

            result["version"] = "testing";

            return result;
        }
    );

    server.addProcessor(
        HTTPRequest::Method::GET,
        "/api/exception",
        [](RESTServer::Arguments args, std::byte* data, std::size_t s) -> nlohmann::json
        {
            throw std::runtime_error("Sample exception");
        }
    );

    server.exec(INADDR_ANY, static_cast<uint16_t>(port));

    return 0;
}