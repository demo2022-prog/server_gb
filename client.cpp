#include <iostream>
#include <sstream>
#include <time.h>
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

std::string get_string_time() {
    time_t ttime;
    struct tm * timeinfo;
    time(&ttime);
    timeinfo = localtime (&ttime);
    std::ostringstream  ss;
    int temp[3] = {timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec};
    for(int i=0; i<3; ++i){
        if(temp[i] < 10) {
            ss << "0";
        }
        ss << temp[i];
        if(i < 2) {
            ss << ":";
        }
    }
    return ss.str();
}

int main() {
    char buffer[1024] = {0};
    std::cin >> buffer;

    socket_wrapper::SocketWrapper sock_wrap;
    const int port = 11111;
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(port)};
    addr.sin_addr.s_addr = my_inet_addr(127,0,0,1);

    socket_wrapper::Socket sock(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (!sock) {
        return EXIT_FAILURE;
    }
    if (connect(sock, reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) < 0) {
        return EXIT_FAILURE;
    }

    strcat(buffer, " >> ");
    strcat(buffer, get_string_time().c_str());
    strcat(buffer, " >> ");

    send(sock, buffer, strlen(buffer), 0);
    auto recv_bytes = recv(sock, buffer, sizeof(buffer), 0);
    buffer[recv_bytes] = '\0';

    strcat(buffer, " << ");
    strcat(buffer, get_string_time().c_str());

    std::cout << buffer << std::endl;
    return 0;
}