//
// Created by dmitri on 01.03.2022.
//

#ifndef SERVER_SOKET_H
#define SERVER_SOKET_H

#include <winsock2.h>
#include <cstdint>

class MySocket {
private:
    WSADATA wsaData;
    SOCKET my_socket;
    sockaddr_in addr_server;
    sockaddr_in addr_client;
    int size_addr_client;
    int max_len;
    char * buffer;
public:
    MySocket(uint32_t port) {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        addr_server.sin_family = AF_INET;
        addr_server.sin_addr.s_addr = INADDR_ANY;
        addr_server.sin_port = htons(port);
        max_len = 256;
        buffer = new char[max_len];
        size_addr_client = -1;
        if(bind(my_socket, (sockaddr*)&addr_server, sizeof(addr_server)) != SOCKET_ERROR)
            size_addr_client = sizeof(addr_client);
    }

    ~MySocket() {
        closesocket(my_socket);
        WSACleanup();
        delete[] buffer;
    }

    bool isReady() {
        return size_addr_client != -1;
    }

    char* getBuffer() {
        return buffer;
    }

    int getError() {
        return WSAGetLastError();
    }

    void setClient(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t port) {
        d = d << 24;
        c = c << 16;
        b = b << 8;
        a = a | b | c | d;
        addr_client.sin_family = AF_INET;
        addr_client.sin_addr.s_addr = a;
        addr_client.sin_port = htons(port);
    }

    bool read() {
        int n = recvfrom(my_socket, buffer, max_len, 0, (sockaddr*)&addr_client, &size_addr_client);
        if(n <= 0)
            return false;
        buffer[n] = '\0';
        return true;
    }

    void write() {
        sendto(my_socket, buffer, strlen(buffer), 0, (sockaddr*)&addr_client, sizeof(addr_client));
    }
};

#endif //SERVER_SOKET_H
