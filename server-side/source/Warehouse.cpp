//
// Created by root on 25/04/19.
//

#include "../include/Warehouse.h"

int Warehouse::run(char* host, int port) {

    cout << "[Warehouse] Creating a new Server" << std::endl;
    Server newServer;
    newServer.createSocket(host, port);
    newServer.run();
    //newServer.receive_file();

    return 0;
}
