#pragma once

#ifdef OS_LINUX

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <zconf.h>

#define INVALID_SOCKET (-1)

using socket_t = int;
#endif
#ifdef OS_WINDOWS
#include "winsock2.h"
#include "ws2tcpip.h"

using socket_t = SOCKET;
#endif