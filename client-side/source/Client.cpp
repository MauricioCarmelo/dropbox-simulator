#include "../include/Client.h"
#include "../../utils/include/Instruction.h"

Client::Client() {}


Client::Client(char *host, int port)
{
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "[Client] ERROR, no such host\n");
        exit(0);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        cout << " [Client] ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
}

int Client::establishConnectionToHost()
{
    if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        cout << " [Client] ERROR connecting" << std::endl;

    return 0;
}

int Client::establishConnectionType(connection_t c)
{
    int n;
    char response[100];
    bzero(response, 100);
    n = write(sockfd, &c, sizeof(struct connection));

    n = read(sockfd, response, 100);

    if (n < 0)
        cout << " [Client] ERROR reading from socket on  establishConnectionType()" << std::endl;

    if (strcmp(response, "ack") == 0)
        return -1;
    else if (strcmp(response, "nack1") == 0) // not possible to insert user
        return -1;
    else if (strcmp(response, "nack2") == 0) // max number of users reached
        return -1;
    else if (strcmp(response, "nack3") == 0) // max number of devices reached
        return -1;
    else if (strcmp(response, "nack4") == 0) // this device is already connected
        return -1;
    else if (strcmp(response, "nack5") == 0) // user inserted but error while inserting device
        return -1;
    //n = write(sockfd, buffer, 7);



    //close(sockfd);
    return 0;
}

int Client::send(file_t data, int size)
{

    /* send command to warn server that a data packet will be sent */
    // send_command_packet();

    /* send the packet */
    packet data_packet = prepare_data_packet(data, size);
    send_data_packet(data_packet);

    return 0;
}

packet Client::prepare_data_packet(file_t data, int size)
{
    packet data_packet;

    data_packet.length = size;
    data_packet.payload = &data;//(char*)malloc(size);
    memcpy(data_packet.payload->name, data.name, strlen(data.name));
    memcpy(data_packet.payload->content, data.content, size - strlen(data.name));

    return data_packet;
}

int Client::send_data_packet(packet data_packet)
{
    int returnFromWrittenSize;
    int ackReturn;
    char ackBuffer[sizeof(uint16_t)];
    char buffer[BUFFER_SIZE];
    int bytesCopiedFromPayload = 0;
    int bytesWritenInSocket = 0;
    int bytesWritenInCurrentIteration = 0;
    int totalSize = data_packet.length;
    int bufferSize;

    returnFromWrittenSize = write(sockfd, &data_packet.length, sizeof(uint16_t));
    if (returnFromWrittenSize != sizeof(uint16_t)) {
        cout << "nao to enviando o length direito" << endl;
    }
    ackReturn = read(sockfd, ackBuffer, sizeof(uint16_t));
    if (ackReturn == -1) {
        cout << "nao recebi ack direito" << endl;
    }

    // send name first


    // send content
    do {
        bufferSize = determineCorrectSizeToBeCopied(totalSize, bytesWritenInSocket);

        char buffertest[data_packet.length];
        memcpy(buffertest, data_packet.payload, data_packet.length);

        memcpy(buffer, data_packet.payload + bytesCopiedFromPayload, bufferSize);
        bytesCopiedFromPayload += bufferSize;

        bytesWritenInCurrentIteration = write(sockfd, buffer, bufferSize);
        if (bufferSize != bytesWritenInCurrentIteration) {
            cout << "Error writing current buffer in socket - should retry this part" << endl;
        }

        bytesWritenInSocket += bytesWritenInCurrentIteration;

    } while (bytesWritenInSocket < totalSize);

    ackReturn = read(sockfd, ackBuffer, sizeof(uint16_t));
    if (ackReturn == -1) {
        cout << "nao recebi ack direito" << endl;
    }

    close(sockfd); // Socket won't close here in production mode*/
    return 0;
}

int Client::determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket) {
    if( totalSize - bytesWritenInSocket >= BUFFER_SIZE )
        return BUFFER_SIZE;
    else
        return totalSize - bytesWritenInSocket;
}

