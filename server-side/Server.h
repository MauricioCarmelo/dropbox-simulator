//
// Created by vagrant on 4/20/19.
//

#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../client-side/include/File.h"
#include "../client-side/include/Client.h"

#define BUFFER_SIZE 4

class Server {

private:

    // We will need arrays of sockets here in order to handle multiple users

    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /*int serverSocket;
    struct sockaddr_in serverAddress;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    //File file_info;*/
    Client client;

public:

    Server();
    ~Server() {};
    //Server(const File &fileInfo, const Client &client);
    int createSocket(char* host, int port);
    int receive_file();
    //int receive_file_stream(char *stream);
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