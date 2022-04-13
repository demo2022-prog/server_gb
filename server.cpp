#include <iostream>
#include <time.h>
#include <thread>
#include <queue>

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

class My_Thread {
public:
    My_Thread(int s):sock(s), state(true) {}
    ~My_Thread() {
        thread->join();
        delete thread;
    }

    void init(std::thread * th) {
        thread = th;
    }

    bool isNull() {
        return !state;
    }

    void setNull() {
        state = false;
    }

    int getSock() {
        return sock;
    }

private:
    std::thread * thread;
    bool state;
    int sock;
};



void th_process(My_Thread * th) {
    srand(time(0));
    char buffer[1024] = {0};
    int sock = th->getSock();
    auto recv_bytes = recv(sock, buffer, sizeof(buffer), 0);
    buffer[recv_bytes] = '\0';
    std::cout << buffer << std::endl;
    int n = rand()%9 + 1;
    strcat(buffer, std::to_string(n).c_str());
    Sleep(n * 1000);
    send(sock, buffer, strlen(buffer), 0);
    th->setNull();
}

int main()
{
    socket_wrapper::SocketWrapper sock_wrap;
    const int port = 11111;
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(port)};
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    socket_wrapper::Socket sock(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock) {
        return EXIT_FAILURE;
    }

    if(bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        throw std::runtime_error("bind()");
    }

    listen(sock, 3);

    std::queue <My_Thread *> qu;

    while(true) {
        struct sockaddr_storage client_addr = {0};
        socklen_t client_len = sizeof(client_addr);
        int newsock = accept(sock, reinterpret_cast<sockaddr *>(&client_addr), &client_len);
        if (newsock < 0) break;

        My_Thread * th = new My_Thread(newsock);
        th->init(new std::thread(th_process, th));
        qu.push(th);
        while(!qu.empty()) {
            th = qu.front();
            if(th->isNull()) {
                delete th;
                qu.pop();
            }
            else break;
        }

    }
    return 0;
}