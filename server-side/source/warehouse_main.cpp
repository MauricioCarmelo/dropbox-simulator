#include <iostream>
#include <sys/socket.h>
#include "../include/Server.h"
#include "../include/Warehouse.h"


int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Please use" << std::endl;
        std:: cout << "./warehouse <port>" << std::endl;
        exit(1);
    }

    int port = atoi(argv[1]);

    Warehouse warehouse;
    warehouse.run(port);

    return 0;
}