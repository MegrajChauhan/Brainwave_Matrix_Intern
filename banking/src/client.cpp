#include "client.hpp"

bool bank::make_connection()
{
    struct sockaddr_in serv_addr;
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }

    u_long mode = 1;
    if (ioctlsocket(sock, FIONBIO, &mode) != 0)
    {
        closesocket(sock);
        WSACleanup();
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        closesocket(sock);
        WSACleanup();
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if (WSAGetLastError() != WSAEWOULDBLOCK)
        {
            closesocket(sock);
            WSACleanup();
            return false;
        }
    }
#elif defined(__linux)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return false;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        close(sock);
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if (errno != EINPROGRESS)
        {
            close(sock);
            return false;
        }
    }
#endif
    std::string msg = get_reply();
    if (msg.empty() || msg[0] != _CONNECTION_MADE)
    {
        size_t len = msg.length();
        return false;
    }
    return true;
}

void bank::send_msg(msg m)
{
    send(sock, m.as_bytes, sizeof(m.as_bytes), 0);
}

std::string bank::get_reply()
{
    std::string reply;

    // Read until we have nothing to read or encounter an error
    size_t len = 0;
    read(sock, reinterpret_cast<char*>(&len), 8);
    char tmp_buf[len+1];
    read(sock, tmp_buf, len);
    tmp_buf[len] = '\0';
    reply.assign(tmp_buf, len);
    return reply;
}

void bank::close_connection()
{
    close(sock);
}

void bank::send_raw(std::string msg)
{
    send(sock, msg.c_str(), msg.length(), 0);
}