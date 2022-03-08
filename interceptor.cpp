#include <iostream>
#include <fstream>
#include "socket.h"

int main(int argc, const char* const argv[]) {

    if(argc != 2)
        return -1;
    MySocket my_socket(argv[1]);
    if(!my_socket.isReady()) {
        std::cout << "Error init: " << my_socket.getError() << std::endl;
        return -1;
    }
    if(!my_socket.capture()) {
        std::cout << "Error capture`: " << my_socket.getError() << std::endl;
        return -1;
    }
    std::ofstream o_file("log.txt");
    char* temp = my_socket.getBuffer();
    o_file.write(temp, strlen(temp));
    o_file.flush();
    o_file.close();

    std::cout << "End capture" << std::endl;
    return 0;
} 