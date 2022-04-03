#include <iostream>
#include <chrono>
#include <thread>

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
    using namespace std::chrono_literals;
    socket_wrapper::SocketWrapper sock_wrap;
    const int port  = 11111;
    std::cout << "Running sending on the port " << port << "...\n";
    struct sockaddr_in addr = {.sin_family = PF_INET, .sin_port = htons(port)};
    addr.sin_addr.s_addr = my_inet_addr(127, 255, 255, 255);
    socket_wrapper::Socket sock(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (!sock) {
        return EXIT_FAILURE;
    }

    char broadcast = 1;

    if (-1 == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))) throw std::runtime_error("setsockopt()");
    std::string message = {"Test broadcast messaging!"};
    while (true) {
        std::cout << "Sending message to broadcast..." << std::endl;
        sendto(sock, message.c_str(), message.length(), 0,
               reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in));
        std::cout << "Message was sent..." << std::endl;
        std::this_thread::sleep_for(1s);
    }
    return 0;
}