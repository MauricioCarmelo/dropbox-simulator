//
// Created by root on 25/04/19.
//

#include "../include/Warehouse.h"

int Warehouse::run(int port) {

    cout << "[Warehouse] Creating a new Server" << std::endl;
    Server newServer;
    newServer.createSocket("127.0.0.1", port);
    newServer.run();
    //newServer.receive_file();

    return 0;
}
