#include "socket.h"
#include <iostream>
#include <thread>


void th_listen(MySocket& my_socket) {
    char* temp = my_socket.getBuffer();
    while(my_socket.read()) {
        if(temp[0] == 1)
            return;
        std::cout << temp << std::endl;
    }
    std::cout << "Recv Error: " << my_socket.getError() << std::endl;
}

void th_input(MySocket& my_socket) {
    char * temp = my_socket.getBuffer();
    while(true) {
        std::cin >> temp;
        if(std::string(temp) == "end") {
            temp[0] = 1;
            my_socket.setClient(127, 0, 0, 1, 22222);
            my_socket.write();
            break;
        }
        my_socket.setClient(127, 0, 0, 1, 11111);
        my_socket.write();
    }
}

int main() {
    MySocket my_socket(22222);
    if(!my_socket.isReady()) {
        std::cout << "Bind Error: " << my_socket.getError() << std::endl;
        return -1;
    }
    std::thread th1(th_listen, std::ref(my_socket));
    std::thread th2(th_input, std::ref(my_socket));
    th1.join();
    th2.join();
    std::cout << "Close client" << std::endl;
    return 0;
}