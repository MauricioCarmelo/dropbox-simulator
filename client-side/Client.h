//
// Created by root on 15/04/19.
//

#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H



class Client {
private:
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
};


#endif //DROPBOX_CLIENT_H
