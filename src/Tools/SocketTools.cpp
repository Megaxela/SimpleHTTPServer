#include "Tools/SocketTools.hpp"

#include <Tools/SocketTools.h>
#include <Tools/ThreadNames.h>
#include <CurrentLogger.hpp>
#include <fcntl.h>

bool ::SocketTools::makeSocketNonBlocking(socket_t socket)
{
#ifdef OS_LINUX
    int flags = fcntl(socket, F_GETFL, 0);

    if (flags == -1)
    {
        return false;
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        return false;
    }

    flags = fcntl(socket, F_GETFL, 0);
    if ((flags & O_NONBLOCK) == O_NONBLOCK)
    {
        return true;
    }

    return false;
#endif
#ifdef OS_WINDOWS
    u_long mode = 1;

    auto result = ioctlsocket(socket, FIONBIO, &mode);
    if (result != NO_ERROR)
    {
        ErrorF() << '[' << ThreadNames::getThisThreadName() << "] Can't set socket to non blocking. Error: " << WSAGetLastError();
        return false;
    }

    return true;
#endif
}

ssize_t SocketTools::Receive(socket_t socket, uint8_t *buffer, int len, int flags)
{
#ifdef OS_LINUX
    return ::recv(socket, buffer, static_cast<size_t>(len), flags);
#endif
#ifdef OS_WINDOWS
    return ::recv(socket, reinterpret_cast<char *>(buffer), len, flags);
#endif
}

ssize_t SocketTools::Send(socket_t socket, const uint8_t* buffer, int len, int flags)
{
#ifdef OS_LINUX
    return ::send(socket, buffer, static_cast<size_t>(len), flags);
#endif
#ifdef OS_WINDOWS
    return ::send(socket, reinterpret_cast<const char *>(buffer), len, flags);
#endif
}

void SocketTools::Close(socket_t socket)
{
#ifdef OS_LINUX
    close(socket);
#endif
#ifdef OS_WINDOWS
    close(static_cast<int>(socket));
#endif
}
