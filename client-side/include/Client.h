
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

    std::string name;
    bool isLogged;
};

#endif //DROPBOX_CLIENT_H