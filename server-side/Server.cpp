//
// Created by vagrant on 4/20/19.
//

#include "Server.h"
#include <cstring>

Server::Server() = default;

int Server::createSocket(char* host, int port) { //TODO host nao é usado aqui

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        printf("ERROR on binding");

    listen(sockfd, 5);

    clilen = sizeof(struct sockaddr_in);

    return 0;
}

int Server::receive_file() {

    char message[10];
    int file_size = 7;
    int bytes_received;
    char buffer[BUFFER_SIZE];

    if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
        printf("ERROR on accept");

    bytes_received = 0;
    while(bytes_received < file_size) {
        bzero(buffer, BUFFER_SIZE);

        /* read from the socket */
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            printf("ERROR reading from socket");

        memcpy(message+bytes_received, buffer, n);
        bytes_received = bytes_received + n;
        std::cout << "Bytes received: " << bytes_received << std::endl;
    }

    n = write(newsockfd, "Hi", 3);
    if (n < 0)
        printf("ERROR writing to socket");

    for (char c: message)
        std::cout << c;
    std::cout << std::endl;

    close(newsockfd);
    close(sockfd);
    return 0;
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


/*
void Server::sync_server() {

}


void Server::send_file(File file, int socket) {

}
*/




