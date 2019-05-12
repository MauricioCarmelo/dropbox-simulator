//
// Created by vagrant on 4/20/19.
//

#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../../client-side/include/File.h"
#include "../../client-side/include/Client.h"
#include "../../utils/masterInclude.h"

#define BUFFER_SIZE 4   // fazer um buffer maior, 256

#define MAX_DEVICES 2
#define MAX_USERS 5

struct Device {
    int id;
    int socket1;
    int socket2; // escolher um nome melhor pros sockets
    int socket3;
};

struct User {
    char nome[25];
    Device devices[MAX_DEVICES];
};


class Server {

private:

    // We will need arrays of sockets here in order to handle multiple users

    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    Client clients[10];
    int semaphore = 0; // we should need this in the future
    pthread_t threads[50];
    static User users[MAX_USERS];

public:

    Server();
    ~Server() {};
    //Server(const File &fileInfo, const Client &client);
    int createSocket(char* host, int port);
    int receive_file();
    int countUserConnections(std::string user); //counts how many connections a user has
    void createUserDirectory(const char *user);
    //int receive_file_stream(char *stream);
    int run();
    int handle_type(int s);
    static void *mediatorThread(void *arg);
    static void *terminalThreadFunction(void *arg);
    static void *iNotifyThreadFunction(void *arg);
    static void *serverNotifyThreadFunction(void *arg);
/*
    void sync_server();                 //TODO Syncs server with directory "sync_dir_<nomeusuário>" and client

    void send_file(File file, int socket);
    const File &getFileInfo() const;
    void setFileInfo(const File &fileInfo);
    const Client &getClient() const;
    void setClient(const Client &client);
    // TODO Sends file to client - filename.ext
    */
};

#endif //DROPBOX_SERVER_H