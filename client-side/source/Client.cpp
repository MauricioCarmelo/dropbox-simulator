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
    char buffer[100];
    //char frase[] = "frase1";
    //memcpy(buffer, frase, 7);
    n = write(sockfd, &c, sizeof(struct connection));
    n = read(sockfd, buffer, 100);


    //n = write(sockfd, buffer, 7);


    if (n < 0)
        cout << " [Client] ERROR reading from socket on  establishConnectionType()" << std::endl;

    //close(sockfd);
    return 0;
}

int Client::sendFile(char *filename, int size, char *fileContent)
{
    filePacket file_packet = prepareFilePacket(filename, size, fileContent);
    sendFilePacket(file_packet);

    return 0;
}

filePacket Client::prepareFilePacket(char *filename, int size, char *fileContent)
{
    filePacket file_packet;

    file_packet.packetType = FILE;
    file_packet.fileSize = size;
    memcpy(file_packet.fileName, filename, strlen(filename));
    file_packet.payload = (char*)malloc(size);
    memcpy(file_packet.payload, fileContent, size);

    return file_packet;
}

int Client::sendFilePacket(filePacket file_packet)
{
    sendDataToSocket(&file_packet.packetType, sizeof(uint64_t));
    waitForSocketAck();
    sendDataToSocket(&file_packet.fileSize, sizeof(uint64_t));
    waitForSocketAck();
    sendDataToSocket(file_packet.fileName, strlen(file_packet.fileName));
    waitForSocketAck();
    sendLargePayloadToSocket(file_packet.payload, file_packet.fileSize);
    waitForSocketAck();

    return 0;
}

int Client::sendDataToSocket(void *data, size_t size) {
    int bytesSocketReceived = write(sockfd, data, size);
    if (bytesSocketReceived != size) {
        cout << "sendDataToSocket: Failed to send data to socket" << endl;
    }
    return bytesSocketReceived;
}

void Client::waitForSocketAck() {
    char ackBuffer[sizeof(uint64_t)];
    int ackReturn = read(sockfd, ackBuffer, sizeof(uint64_t));
    if (ackReturn == -1) {
        cout << "waitForSocketAck: Failed to receive ack" << endl;
    }
}

int Client::sendLargePayloadToSocket(char *data, size_t totalSize) {
    char buffer[BUFFER_SIZE];
    int bytesCopiedFromPayload = 0;
    int bytesWritenInSocket = 0;
    int bytesWritenInCurrentIteration = 0;
    int bufferSize;
    do {
        bufferSize = determineCorrectSizeToBeCopied(totalSize, bytesWritenInSocket);

        memcpy(buffer, data + bytesCopiedFromPayload, bufferSize);
        bytesCopiedFromPayload += bufferSize;

        bytesWritenInCurrentIteration = sendDataToSocket(buffer, bufferSize);
        if (bufferSize != bytesWritenInCurrentIteration) {
            cout << "Client.sendFilePacket: Error writing current buffer in socket" << endl;
        }

        bytesWritenInSocket += bytesWritenInCurrentIteration;

    } while (bytesWritenInSocket < totalSize);
    return bytesWritenInSocket;
}

int Client::determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket) {
    if( totalSize - bytesWritenInSocket >= BUFFER_SIZE )
        return BUFFER_SIZE;
    else
        return totalSize - bytesWritenInSocket;
}

