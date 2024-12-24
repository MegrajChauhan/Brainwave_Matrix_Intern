#ifndef _CLIENT_
#define _CLIENT_

#include <string>
#include <cstring>
#include <unordered_map>
#include "defs.hpp"
#include "API.hpp"

#define PORT 8080

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

WSADATA wsaData;
SOCKET sock = INVALID_SOCKET;
#elif defined(__linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
static int sock;
#endif

namespace bank
{
    union msg
    {
        char as_bytes[16];
        struct
        {
            uint64_t as_long1;
            uint64_t as_long2;
        };
    };

    bool make_connection();

    void send_msg(msg m);

    std::string get_reply();

    void close_connection();

    void send_raw(std::string msg);
};

#endif