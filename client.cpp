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
    addr.sin_addr.s_addr = my_inet_addr(127,0,0,1);

    socket_wrapper::Socket sock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock) {
        return EXIT_FAILURE;
    }
    if (connect(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0) {
        return EXIT_FAILURE;
    }

    char buffer[1024] = {0};
    std::cin >> buffer;
    send(sock, buffer, strlen(buffer), 0);
    auto recv_bytes = recv(sock, buffer, sizeof(buffer), 0);
    buffer[recv_bytes] = '\0';
    std::cout << buffer << std::endl;
    return 0;
}