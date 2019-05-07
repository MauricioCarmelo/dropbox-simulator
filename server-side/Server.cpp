//
// Created by vagrant on 4/20/19.
//

#include "Server.h"
#include <cstring>
#include <sys/stat.h>

Server::Server() = default;

int Server::createSocket(char* host, int port) { //TODO host nao é usado aqui

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        std:: cout << "ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        std:: cout << "ERROR on binding" << std::endl;

    listen(sockfd, 5);

    clilen = sizeof(struct sockaddr_in);

    return 0;
}

int Server::run() {
    while (1){
        std::cout << "opa1" << std::endl;
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
            std::cout << "ERROR on accept" << std::endl;
        auto s = newsockfd;
        std::cout << "opa2" << std::endl;

        //std::thread t1(&Server::handle_type, );

        handle_type(s);
    }
}

int Server::handle_type(int s)
{
    connection_t c;

    //char buffer[7];
    //bzero(buffer, 7);
    std::cout << "opa3" << std::endl;
    n = read(s, &c, sizeof(struct connection));
    //n = read(newsockfd, buffer, 7);
    std::cout << "opa4" << std::endl;
    //std::cout << buffer << std::endl;
    std::cout << c.type << std::endl;
    if (n < 0) {
        std::cout << "ERROR reading from socket" << std::endl;
        return 1;
    }
    n = write(s, "ack", 3);

    if(c.type == T1)
        std::cout << "Tipo 1" << std::endl;
    else if(c.type == T2)
        std::cout << "Tipo 2" << std::endl;
    else
        std::cout << "Tipo nao definido" << std::endl;

    return 0;
}

int Server::receive_file() {

    char message[100];  // isso nao vai ficar aqui
    int file_size = 7;  // determinar o tamanho do arquivo de outra maneira
    int bytes_received;
    char buffer[BUFFER_SIZE];

    if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
        std:: cout << "ERROR on accept" << std::endl;

    bytes_received = 0;
    while(bytes_received < file_size) {
        bzero(buffer, BUFFER_SIZE);

        /* read from the socket */
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            std:: cout << "ERROR reading from socket" << std::endl;

        memcpy(message+bytes_received, buffer, n);
        bytes_received = bytes_received + n;
        //std::cout << "Bytes received: " << bytes_received << std::endl;
    }
    n = write(newsockfd, "ack", 3);
    if (n < 0)
        std:: cout << "ERROR writing to socket" << std::endl;

    for (int i=0; i<bytes_received; i++){
        std::cout << message[i];
    }
    std::cout << std::endl;

    /*for (char c: message) {
        std::cout << c;
    }*/

    //close(newsockfd);
    //close(sockfd);
    return bytes_received;
}

int Server::countUserConnections(std::string user) {
    int i, count = 0;

    while (semaphore == 1){
        //do nothing
    }

    semaphore = 1;

    for (i = 0; i < 10; i++){
        if (clients[i].name == user && clients[i].isLogged)
            count++;
    }

    std:: cout << "[Server] User " << user << "has " << count << "connections" << std::endl;

    semaphore = 0;

    return count;
}

void Server::createUserDirectory(const char *user) {

    struct stat st = {0};
    char diretorio[50] = "sync_dir_";
    strcat(diretorio,user);

    if (stat(diretorio, &st) != 0) {
        mkdir(diretorio, 07777);
        std:: cout << "[Server] User: " << user << "has now a client folder" << std::endl;
    }
    else {
        std::cout << "[Server] User " << "already has a folder" << std::endl;
    }

}



/*
void Server::sync_server() {

}


void Server::send_file(File file, int socket) {

}
*/




