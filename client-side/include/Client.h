
#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#define BUFFER_SIZE 4
#define CMD 1
#define DATA 2

typedef struct packet{
    int16_t type;
    uint16_t length;
    char* payload;
} packet;

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
    packet prepare_data_packet(char *data, int size);
    int send_data_packet(packet data_packet);
};

#endif //DROPBOX_CLIENT_H