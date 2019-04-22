//
// Created by vagrant on 4/20/19.
//

#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H


#include "../client-side/include/File.h"
#include "../client-side/include/Client.h"

class Server {

private:

    File file_info;
    Client client;

public:

    Server();

    Server(const File &fileInfo, const Client &client);

    void sync_server();                 //TODO Syncs server with directory "sync_dir_<nomeusuário>" and client

    void receive_file(File file, int socket);      // TODO Receives a file from client. Should be executed when receiving a file file - path/filename.ext

    int createServerSocket(std::string host, int port);

    void send_file(File file, int socket);

    const File &getFileInfo() const;

    void setFileInfo(const File &fileInfo);

    const Client &getClient() const;

    void setClient(const Client &client);
    // TODO Sends file to client - filename.ext
};


#endif //DROPBOX_SERVER_H
