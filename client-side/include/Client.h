
#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include "../../utils/masterInclude.h"


#define BUFFER_SIZE 4
#define CMD 1
#define DATA 2

#define T1 1
#define T2 2
#define T3 3

using namespace std;

typedef struct packet{
    int16_t type;
    uint16_t length;
    char* payload;
} packet;

typedef struct connection{
    int16_t type;
    int16_t username; // mudar pra char[]
    // mandar o id do device
} connection_t;

class Client {
private:
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
public:
    Client();
    ~Client() {};
    Client(char *host, int port);
    int establishConnectionToHost();
    int send(char* buffer, int size);

    int establishConnectionType(connection_t c);

    std::string name;
    bool isLogged;
    packet prepare_data_packet(char *data, int size);
    int send_data_packet(packet data_packet);
};

#endif //DROPBOX_CLIENT_H