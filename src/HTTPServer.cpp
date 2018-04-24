#include <CurrentLogger.hpp>
#include <Tools/SocketTools.hpp>
#include "HTTPServer.hpp"

HTTPServer::HTTPServer() :
    m_recvSocket(0),
    m_requestBuffer()
{
    Info() << "HTTP Server created.";
}

HTTPServer::~HTTPServer()
{
    if (m_recvSocket)
    {
        SocketTools::Close(m_recvSocket);
    }
}

bool HTTPServer::initializeSocket(uint32_t address, uint16_t port)
{
    m_recvSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_recvSocket == INVALID_SOCKET)
    {
        Error() << "Can't initialize socket. Error: " << strerror(errno);
        return false;
    }

    sockaddr_in addr{0};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(address);

    if (bind(m_recvSocket, (const sockaddr*) &addr, sizeof(sockaddr_in)) == -1)
    {
        Error() << "Can't bind socket. Error: " << strerror(errno);
        SocketTools::Close(m_recvSocket);
        return false;
    }

    if (listen(m_recvSocket, 32) == -1)
    {
        Error() << "Can't set socket listen. Error: " << strerror(errno);
        SocketTools::Close(m_recvSocket);
        return false;
    }

    return true;
}

void HTTPServer::exec(uint32_t address, uint16_t port)
{
    Info() << "Initializing socket at port " << port << "...";

    if (!initializeSocket(address, port))
    {
        Error() << "Initialization failed.";
        return;
    }

    Info() << "Initialization succeed.";

    socket_t clientSocket = 0;
    sockaddr_in client{0};
    HTTPRequest request;

    while (true) // Endless loop
    {
        socklen_t len = sizeof(client);

        // Awaiting client
        clientSocket = accept(m_recvSocket, (sockaddr*) &client, &len);

        if (clientSocket == -1)
        {
            Error() << "Can't accept new connection. Error: " << strerror(errno);
            SocketTools::Close(m_recvSocket);
            return;
        }

        auto o1 = static_cast<std::byte>((client.sin_addr.s_addr & 0xFF000000U) >> (3 * 8));
        auto o2 = static_cast<std::byte>((client.sin_addr.s_addr & 0x00FF0000U) >> (2 * 8));
        auto o3 = static_cast<std::byte>((client.sin_addr.s_addr & 0x0000FF00U) >> (1 * 8));
        auto o4 = static_cast<std::byte>((client.sin_addr.s_addr & 0x000000FFU) >> (0 * 8));

        Info() << "Received connection from "
            << (int) o4 << '.' << (int) o3 << '.' << (int) o2 << '.' << (int) o1;

        // Reading request
        if (!readRequest(clientSocket, request))
        {
            Warning() << "Wrong request received from "
                         << (int) o4 << '.' << (int) o3 << '.' << (int) o2 << '.' << (int) o1;
            SocketTools::Close(clientSocket);

            continue;
        }

        HTTPResponse response = proceedRequest(std::move(request));

        m_requestBuffer.resize(response.calculateSerializedSize(), std::byte(0));
        response.serialize(m_requestBuffer.data());

        SocketTools::Send(
            clientSocket,
            reinterpret_cast<const uint8_t*>(m_requestBuffer.data()),
            static_cast<int>(response.calculateSerializedSize()),
            0
        );

        SocketTools::Close(clientSocket);
    }
}

bool HTTPServer::readRequest(socket_t client, HTTPRequest& request)
{
    m_requestBuffer.clear();
    if (m_requestBuffer.size() < 1024)
    {
        m_requestBuffer.resize(1024, std::byte(0));
    }

    std::size_t received = 0;

    std::size_t crlf = 0;

    bool finish = false;

    while (!finish)
    {
        ssize_t currentlyReceived = SocketTools::Receive(
            client,
            (uint8_t*) m_requestBuffer.data() + received,
            1,
            0
        );

        if (currentlyReceived == -1)
        {
            Error() << "Received error: " << strerror(errno);
            return false;
        }

        received += currentlyReceived;

        if (received >= m_requestBuffer.size())
        {
            Info() << "Resizing..." << std::endl;
            m_requestBuffer.resize(
                m_requestBuffer.size() + 1024,
                std::byte(0)
            );
        }

        switch (crlf)
        {
        case 0:
            if (m_requestBuffer[received - 1] == std::byte('\r'))
            {
                crlf = 1;
            }
            break;
        case 1:
            if (m_requestBuffer[received - 1] == std::byte('\n'))
            {
                crlf = 2;
            }
            else
            {
                crlf = 0;
            }
            break;
        case 2:
            if (m_requestBuffer[received - 1] == std::byte('\r'))
            {
                crlf = 3;
            }
            else
            {
                crlf = 0;
            }
            break;
        case 3:
            if (m_requestBuffer[received - 1] == std::byte('\n'))
            {
                finish = true;
            }
            else
            {
                crlf = 0;
            }
            break;

        default:

            break;
        }
    }

    return request.parse(m_requestBuffer.data(), received);
}

HTTPResponse HTTPServer::proceedRequest(HTTPRequest request)
{
    std::cout << "URI: " << request.uri() << std::endl;
    std::cout << "HTTP: " << request.version() << std::endl;
    std::cout << "Headers:" << std::endl;

    for (HTTPHeader::HeadersContainer::size_type i = 0;
         i < request.header().numberOfHeaders();
         ++i)
    {
        std::cout << "    Name: " << request.header().header(i).first
                  << " Value: " << request.header().header(i).second << std::endl;
    }

    HTTPResponse response;

    response.statusCode() = HTTPResponse::StatusCode::Ok;
    response.version() = "HTTP/1.1";

    return std::move(response);
}