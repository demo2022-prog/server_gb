#include <iostream>
#include <time.h>
#include <thread>
#include <queue>
#include <cstring>

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
    char buffer[1024] = {0};

    socket_wrapper::SocketWrapper sock_wrap;
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(11111)};
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sockaddr_in addr_client;
    int size_addr_client = sizeof(addr_client);

    socket_wrapper::Socket sock(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (!sock) {
        return EXIT_FAILURE;
    }

    if(bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR){
        throw std::runtime_error("bind()");
    }

    while(true)	{
        int n = recvfrom(sock, buffer, 1024, 0, (sockaddr*)&addr_client, &size_addr_client);

        if(n <= 0) {
            return -1;
        }

        buffer[n] = '\0';
        std::cout << buffer << std::endl;
        strcat(buffer, " (answer)");
        sendto(sock, buffer, strlen(buffer), 0, (sockaddr*)&addr_client, sizeof(addr_client));

    }
    return 0;
}

