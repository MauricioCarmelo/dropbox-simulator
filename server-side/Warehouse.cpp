#include <iostream>
#include <sys/socket.h>
#include "Server.h"

int main() {

    int serverSocket, newSocket;
    struct sockaddr_storage serverWarehouse;
    socklen_t addressSize;

    Server newServer;

    serverSocket = newServer.createServerSocket("127.0.0.1", 4200);

    while (true){

        std::cout << "Waiting for client..." << std::endl;

        listen(serverSocket,10);

        addressSize = sizeof serverWarehouse;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverWarehouse, &addressSize);

       // newServer.send_file(testFile,newSocket); TODO send file to server
    }

    return 0;
}

