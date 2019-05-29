#include "../include/Warehouse.h"

int Warehouse::run(char* host, int port) {

    cout << "[Warehouse] Creating a new Server" << std::endl;
    Server newServer;
    newServer.createSocket(host, port);
    newServer.run();

    return 0;
}
