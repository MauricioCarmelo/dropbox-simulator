

#include "Box.h"
#include "Client.h"

int Box::open(char *host, int port) {
    client = Client(host, port);
    client.establishConnectionToHost();
}
