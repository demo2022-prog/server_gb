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
    struct sockaddr_in addr = {.sin_family = PF_INET, .sin_port = htons(port)};
    addr.sin_addr.s_addr = my_inet_addr(127,0,0,1);



    socket_wrapper::Socket sock(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (!sock) {
        return EXIT_FAILURE;
    }
    char broadcast = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));
    if(bind(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr)) == SOCKET_ERROR) throw std::runtime_error("bind()");



    char buffer[256] = {};
    while (true) {
        if (recv(sock, buffer, sizeof(buffer) - 1, 0) < 0) {
            std::cerr << sock_wrap.get_last_error_string() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << buffer << std::endl;
    }
    return 0;
}