//
// Created by vagrant on 4/20/19.
//

#include "Server.h"
#include <cstring>

int Server::createServerSocket(std::string host, int port) {

    int serverSocket;
    struct sockaddr_in serverAddress;

    //First parameter is the internet domain IPV4
    //Second parameter means we are using TCP
    //Third paramer tells the OP to use the default protocol
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY; //TODO use host parameter here instead
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);

    bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)); // Connects socket with server address

    return serverSocket;
}

void Server::sync_server() {

}

void Server::receive_file(File file, int socket) {

}

void Server::send_file(File file, int socket) {

}

Server::Server() = default;

Server::Server(const File &fileInfo, const Client &client) : file_info(fileInfo), client(client) {}





