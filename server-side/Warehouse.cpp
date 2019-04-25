//
// Created by root on 25/04/19.
//

#include "Warehouse.h"

int Warehouse::run(int port) {
    int serverSocket, newSocket;
    struct sockaddr_storage serverWarehouse;
    socklen_t addressSize;

    Server newServer;
    newServer.createSocket("127.0.0.1", port);
    newServer.receive_file();

    /*while (true){

        std::cout << "Waiting for client..." << std::endl;

        listen(serverSocket,10);

        addressSize = sizeof serverWarehouse;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverWarehouse, &addressSize);

       // newServer.send_file(testFile,newSocket); TODO send file to server
    }*/
    return 0;
}
