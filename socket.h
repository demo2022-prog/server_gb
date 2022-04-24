`//
// Created by dmitri on 04.03.2022.
//

#ifndef SERVER_SOKET_H
#define SERVER_SOKET_H

#include <winsock2.h>
#include <mstcpip.h>

class MySocket {
private:
    WSADATA wsaData;
    SOCKET my_socket;
    sockaddr_in addr;
    int max_len;
    char * buffer;

public:
    MySocket(std::string i_num) {
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        my_socket = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
        max_len = 0;
        if(my_socket != INVALID_SOCKET) {
            addr.sin_family = PF_INET;
            addr.sin_port = 0;
            addr.sin_addr.s_addr = getIndex(i_num);
            if(bind(my_socket, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
                if(setPromisc(true)) {
                    max_len = 4000;
                    buffer = new char[max_len];
                }
            }
        }
    }

    ~MySocket() {
        delete[] buffer;
        setPromisc(false);
        closesocket(my_socket);
        WSACleanup();
    }

    bool isReady() {
        return (max_len != 0);
    }

    bool setPromisc(bool en) {
        int value = en ? RCVALL_ON : RCVALL_OFF;
        DWORD out = 0;
        return (WSAIoctl(my_socket, SIO_RCVALL, &value, sizeof(value), nullptr, 0, &out, nullptr, nullptr) != INVALID_SOCKET);
    }

    bool capture() {
        int n = recv(my_socket, buffer, max_len, 0);
        if(n <= 0)
            return false;
        buffer[n] = '\0';
        return true;
    }

    char* getBuffer() {
        return buffer;
    }

    int getError() {
        return WSAGetLastError();
    }

    uint32_t getIndex(std::string num) {
        uint32_t res[4];
        for(int i=0; i<4; ++i) {
            int n = num.find(".");
            if(n < 0)
                res[i] = std::atoi(num.c_str());
            else {
                res[i] = std::atoi(num.substr(0, n).c_str());
                num = num.substr(n+1);
            }
        }
        return (res[0] | res[1] << 8 | res[2] << 16 | res[3] << 24);
    }
};

#endif //SERVER_SOKET_H
