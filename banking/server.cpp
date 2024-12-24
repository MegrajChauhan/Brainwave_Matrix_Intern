#include <iostream>
#include <cstring>
#include <unordered_map>
#include "defs.hpp"
#include "API.hpp"
#include "user.hpp"

#define PORT 8080

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

WSADATA wsaData;
SOCKET ssock = INVALID_SOCKET;
#elif defined(__linux)
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
static int server_fd, ssock;
#endif

static std::unordered_map<accnum_t, accounts::User> __all_logged_in_users;

bool main_loop()
{
    union
    {
        char msg[16];
        uint64_t _msg[2];
    } buffer;

    std::string send_buf = "";

    // first tell the client that they are connected
    send_buf = (char)_CONNECTION_MADE;
    buffer._msg[0] = 1;
    if (send(ssock, reinterpret_cast<char*>(&buffer._msg[0]), 8, 0) < 0)
    {
        std::cerr << "Failed to notify client about connection.\n";
    }
    if (send(ssock, send_buf.c_str(), send_buf.length(), 0) < 0)
    {
        std::cerr << "Failed to notify client about connection.\n";
    }
    else
    {
        std::cout << "Client notified about connection.\n";
    }

    // Main loop to handle requests
    while (true)
    {
        buffer._msg[0] = 0;
        buffer._msg[1] = 0;
        if (read(ssock, &buffer.msg, sizeof(buffer)) <= 0)
        {
            std::cerr << "Error reading from socket. Exiting loop.\n";
            break;
        }
        std::cout << "Received request with header: " << (int)buffer.msg[0] << "\n";

        switch (buffer._msg[0] >> 56)
        {
        case _CLOSE_CONN:
            std::cout << "Client requested to close the connection.\n";
            goto _done;
        case _LOGIN:
        {
            send_buf.clear();
            accnum_t acc = buffer._msg[1];
            std::cout << "Processing login for account: " << acc << "\n";

            if (__all_logged_in_users.find(acc) != __all_logged_in_users.end())
            {
                std::cout << "Account " << acc << " already logged in.\n";
                send_buf = (char)_ALREADY_LOGIN;
                break;
            }

            accounts::User u = fetch::fetch_user(acc);
            if (!fetch::fetch_status())
            {
                std::cerr << "Error fetching user data for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }

            size_t pin = buffer._msg[0] & 0xFFFF;
            std::cout << "PIN: " << pin << "\n";
            std::cout << "Verifying PIN for account: " << acc << "\n";
            if (!u.log_in(pin))
            {
                std::cerr << "Incorrect PIN for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_INCORRECT_PIN;
                break;
            }

            __all_logged_in_users[acc] = u;
            std::cout << "Login successful for account: " << acc << "\n";
            send_buf = (char)_SUCCESS;
            break;
        }
        case _LOGOUT:
        {
            send_buf.clear();
            accnum_t acc = buffer._msg[1];
            std::cout << "Processing logout for account: " << acc << "\n";

            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                std::cerr << "Logout failed. Account " << acc << " not logged in.\n";
                send_buf = (char)_ERROR;
                break;
            }

            if (!__all_logged_in_users[acc].write_back())
            {
                std::cerr << "Error unfetching user data for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }

            __all_logged_in_users.erase(acc);
            std::cout << "Logout successful for account: " << acc << "\n";
            send_buf = (char)_SUCCESS;
            break;
        }
        case _DEPOSIT:
        {
            send_buf.clear();
            accnum_t acc = buffer._msg[1];
            std::cout << "Processing deposit for account: " << acc << "\n";

            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                std::cerr << "Deposit failed. Account " << acc << " not logged in.\n";
                send_buf = (char)_NOT_LOGGED_ON;
                break;
            }

            if (read(ssock, &buffer.msg, sizeof(buffer)) <= 0)
            {
                std::cerr << "Error reading deposit amount from socket.\n";
                send_buf = (char)_ERROR;
                break;
            }

            double amount = static_cast<double>(buffer._msg[0]);
            std::cout << "Attempting to deposit " << amount << " to account: " << acc << "\n";

            accounts::User *u = &__all_logged_in_users[acc];
            if (!u->deposit(amount))
            {
                std::cerr << "Deposit failed for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }

            std::cout << "Deposit successful for account: " << acc << "\n";
            send_buf = (char)_SUCCESS;
            break;
        }
        case _WITHDRAW:
        {
            send_buf.erase();
            accnum_t acc = buffer._msg[1];
            std::cout << "Processing withdraw for account: " << acc << "\n";
            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                std::cerr << "Withdraw failed. Account " << acc << " not logged in.\n";
                send_buf = (char)_NOT_LOGGED_ON;
                break;
            }
            read(ssock, &buffer.msg, sizeof(buffer));
            double amount = static_cast<double>(buffer._msg[0]);
            accounts::User *u = &__all_logged_in_users[acc];
            std::cout << "Attempting to withdraw " << amount << " to account: " << acc << "\n";
            if (!u->withdraw(amount))
            {
                std::cerr << "Withdraw failed for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }
            std::cout << "Withdraw successful for account: " << acc << "\n";
            send_buf = (char)_SUCCESS;
            break;
        }
        case _TRANSFER:
        {
            send_buf.erase();
            accnum_t acc = buffer._msg[1];
            std::cout << "Processing Transfer for account: " << acc << "\n";
            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                std::cerr << "Transfer failed. Account " << acc << " not logged in.\n";
                send_buf = (char)_NOT_LOGGED_ON;
                break;
            }
            read(ssock, &buffer.msg, sizeof(buffer));
            accnum_t receiver = static_cast<accnum_t>(buffer._msg[0]);
            double amount = static_cast<double>(buffer._msg[1]);
            std::cout << "Attempting to Transfer " << amount << " to account: " << receiver << "\n";
            if (!__all_logged_in_users[acc].transfer(receiver, amount))
            {
                std::cerr << "Transfer failed for account: " << acc << "\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }
            std::cerr << "Transfer successful for account: " << acc << "\n";
            send_buf = (char)_SUCCESS;
            break;
        }
        case _REGISTER:
        {
            send_buf.erase();
            std::cout << "Registering New Account\n";
            accnum_t acc = accounts::generate_account_number();
            size_t pin = buffer._msg[0] & 0xFFFF;
            size_t name_len = (buffer._msg[0] >> 16) & 0xFFFF;
            accounts::__account_t type = static_cast<accounts::__account_t>((buffer._msg[0] >> 48) & 0xFF);
            if (__all_logged_in_users.find(acc) != __all_logged_in_users.end())
            {
                // User already exists
                std::cout << "User Already Exists\n";
                send_buf = (char)_USER_EXISTS;
                break;
            }
            char name[name_len + 1];
            read(ssock, name, name_len);
            name[name_len] = 0;
            if (!fetch::create_user(acc, name, pin, type))
            {
                std::cout << "Failed to register new user\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }
            send_buf = (char)_SUCCESS;
            send_buf += std::to_string(acc);
            std::cout << "Registered New User with account number: " << acc << " FOR: " << name << "\n";
            break;
        }
        case _GET_TRANSACTION:
        {
            send_buf.erase();
            accnum_t acc = buffer._msg[1];
            std::cout << "Fetching Transaction Details for: " << acc << "\n";
            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                // User not logged in
                std::cout << "User not logged in.\n";
                send_buf = (char)_NOT_LOGGED_ON;
                break;
            }
            if (!fetch::fetch_user_transaction(acc, &send_buf))
            {
                std::cout << "Failed to obtain transactions\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }
            break;
        }
        case _GET_METADATA:
        {
            send_buf.erase();
            accnum_t acc = buffer._msg[1];
            std::cout << "Fetching Metadata Details for: " << acc << "\n";
            if (__all_logged_in_users.find(acc) == __all_logged_in_users.end())
            {
                // User not logged in
                std::cout << "User not logged in.\n";
                send_buf = (char)_NOT_LOGGED_ON;
                break;
            }
            if (!fetch::fetch_user_metadata(acc, &send_buf))
            {
                std::cout << "Failed to obtain Metadata\n";
                std::cerr << API::get_error()<< "\n";
                send_buf = (char)_ERROR;
                break;
            }
            break;
        }
        default:
            std::cerr << "Unknown header received: " << (int)buffer.msg[0] << "\n";
            send_buf = (char)_ERROR;
            break;
        }

        size_t len = send_buf.length();
        if (send(ssock, reinterpret_cast<char *>(&len), 8, 0) < 0)
        {
            std::cerr << "Failed to send response to client.\n";
        }
        if (send(ssock, send_buf.c_str(), send_buf.length(), 0) < 0)
        {
            std::cerr << "Failed to send response to client.\n";
        }
    }

_done:
    std::cout << "Closing main loop.\n";
    return true;
}

bool make_connection()
{
    struct sockaddr_in serv_addr;
#ifdef _WIN32
    if ((ssock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid serv_addr / serv_addr not supported.\n";
        closesocket(ssock);
        WSACleanup();
        return false;
    }

    if (connect(ssock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection failed. Error: " << WSAGetLastError() << "\n";
        closesocket(ssock);
        WSACleanup();
        return false;
    }
#elif defined(__linux)
    int opt = 1;
    int addrlen = sizeof(serv_addr);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return false;
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return false;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    if ((ssock = accept(server_fd, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed");
        close(server_fd);
        return false;
    }

#endif
    return true;
}

int main()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << "\n";
        return 1;
    }
#endif
    if (!make_connection())
        return -1;

    if (!main_loop())
        return -1;

    close(ssock);

#ifdef __linux
    close(server_fd);
#endif
    return 0;
}
