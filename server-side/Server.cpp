//
// Created by vagrant on 4/20/19.
//

#include "Server.h"
#include <cstring>

Server::Server() = default;

int Server::createSocket(char* host, int port) {

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

    /*if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("ERROR opening socket");
    //First parameter is the internet domain IPV4
    //Second parameter means we are using TCP
    //Third paramer tells the OP to use the default protocol
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY; //TODO use host parameter here instead
    memset(serverAddress.sin_zero, '\0', sizeof serverAddress.sin_zero);
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        printf("ERROR on binding");

    listen(serverSocket, 5);

    clilen = sizeof(struct sockaddr_in);*/

    //return serverSocket;
    return 0;
}

int Server::receive_file() {

    char message[100];
    int file_size = 3;  // determinar o tamanho do arquivo de outra maneira
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
        //std::cout << "Bytes received: " << bytes_received << std::endl;
    }
    n = write(newsockfd, "ack", 3);
    if (n < 0)
        printf("ERROR writing to socket");

    for (int i=0; i<bytes_received; i++){
        std::cout << message[i];
    }
    std::cout << std::endl;

    /*for (char c: message) {
        std::cout << c;
    }*/

    //close(newsockfd);
    //close(sockfd);
    return 0;
}



/*
void Server::sync_server() {

}


void Server::send_file(File file, int socket) {

}
*/




