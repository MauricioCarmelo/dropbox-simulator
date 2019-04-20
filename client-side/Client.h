
#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include <strings.h>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>

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

};


#endif //DROPBOX_CLIENT_H
