#include <iostream>

#include "socket_wrapper/socket_headers.h"
#include "socket_wrapper/socket_wrapper.h"
#include "socket_wrapper/socket_class.h"

uint32_t my_inet_addr(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    d = d << 24;
    c = c << 16;
    b = b << 8;
    a = a | b | c | d;
    return a;
}


int main() {
    socket_wrapper::SocketWrapper sock_wrap;
    const int port = 11111;
    std::cout << "Running sending on the port " << port << "...\n";
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(port)};
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socket_wrapper::Socket sock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock) {
        return EXIT_FAILURE;
    }

    if(bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        throw std::runtime_error("bind()");
    }

    listen(sock, 5);

    while (true) {
        struct sockaddr_storage client_addr = {0};
        socklen_t client_len = sizeof(client_addr);
        int newsock = accept(sock, reinterpret_cast<sockaddr *>(&client_addr), &client_len);

        if (newsock < 0) {
            throw std::runtime_error("ERROR");
        }

        char buffer[1024] = {0};
        auto recv_bytes = recv(newsock, buffer, sizeof(buffer), 0);
        buffer[recv_bytes] = '\0';
        std::cout << buffer << std::endl;

        strcat(buffer, " (answer)");
        send(newsock, buffer, strlen(buffer), 0);
    }

    return 0;
}