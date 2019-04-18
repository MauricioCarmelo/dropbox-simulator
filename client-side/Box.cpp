

#include "Box.h"
#include "Client.h"

int Box::open(char *host, int port) {
    Client client(host, port);
    client.establishConnectionToHost();
}
