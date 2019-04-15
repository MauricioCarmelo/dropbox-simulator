#include "Client.h"

Client::Client(char *host, int port) {
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        std::cout << "ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
}

int Client::establishConnectionToHost() {
    if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        std::cout << "ERROR connecting" << std::endl;

    return 0;
}
