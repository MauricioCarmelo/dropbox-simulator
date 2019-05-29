#include <iostream>
#include <sys/socket.h>
#include "../include/Server.h"
#include "../include/Warehouse.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Please use" << std::endl;
        std:: cout << "./warehouse <port> or ./warehouse <address> <port>" << std::endl;
        exit(1);
    }

    char* host;
    int port;
    if(argc == 2){
        cout << "[Warehouse Main] Using default host: 127.0.0.1" << endl;
        host = (char*)malloc(strlen("127.0.0.1"));
        strcpy(host, "127.0.0.1");
        port = atoi(argv[1]);
    }
    else{
        cout << "[Warehouse Main] Using parameter host: " << argv[1] << endl;
        host = (char*)malloc(strlen(argv[1]));
        strcpy(host, argv[1]);
        port = atoi(argv[2]);
    }

    Warehouse warehouse;
    warehouse.run(host, port);

    return 0;
}