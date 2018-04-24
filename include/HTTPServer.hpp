#pragma once

#include <cstdint>
#include <Tools/Network.hpp>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

class HTTPServer
{
public:

    /**
     * @brief Constructor.
     */
    HTTPServer();

    /**
     * @brief Destructor.
     */
    virtual ~HTTPServer();

    /**
     * @brief Main execution method.
     * @param address 4 byte ipv4 address.
     * @param port Port.
     */
    void exec(uint32_t address, uint16_t port);

protected:

    /**
     * @brief Virtual method for processing
     * http request. By default it's just logging
     * request and returing empty OK response.
     * @param request Request object.
     * @return Response, that has to be sent to request
     * peer.
     */
    virtual HTTPResponse proceedRequest(HTTPRequest request);

private:

    /**
     * @brief Method for initializing server socket.
     * @param address Binding address.
     * @param port Binding port.
     * @return Initializing success.
     */
    bool initializeSocket(uint32_t address, uint16_t port);

    /**
     * @brief Method for reading request from client.
     * @param client Client socket.
     * @param request Output request object.
     * @return Reading success.
     */
    bool readRequest(socket_t client, HTTPRequest& request);

    socket_t m_recvSocket;

    std::vector<std::byte> m_requestBuffer;
};

