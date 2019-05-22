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

int Client::establishConnectionType(connection_t connection)
{
    sendLargePayloadToSocket((char*)&connection, sizeof(struct connection));
    //waitForSocketAck();

    char response[100];
    bzero(response, 100);

    read(sockfd, response, 100);

    if (strcmp(response, "ack") == 0)
        return 0;
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

    return 0;
}

int Client::sendExitCommand() {
    commandPacket command;
    command.packetType = CMD;
    command.command = EXIT;

    sendLargePayloadToSocket((char*)&command, sizeof(struct commandPacket));
    waitForSocketAck();

    return 0;
}

int Client::sendFile(char *filename, int size, char *fileContent)
{
    commandPacket command_packet;
    command_packet.packetType = CMD;
    command_packet.command = UPLOAD;

    sendLargePayloadToSocket((char*)&command_packet, sizeof(struct commandPacket));
    waitForSocketAck();

    filePacket file_packet = prepareFilePacket(filename, size, fileContent);
    sendFilePacket(file_packet);

    return 0;
}

filePacket Client::prepareFilePacket(char *filename, int size, char *fileContent)
{
    filePacket file_packet;

    file_packet.packetType = FILE;
    file_packet.fileSize = size;
    file_packet.fileName = (char*)malloc(size);
    strcpy(file_packet.fileName, filename);
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

int Client::readDataFromSocket(char *buffer, size_t size) {
    int bytesRead = read(sockfd, buffer, size);
    if (bytesRead == -1) {
        cout << "readDataFromSocket: Failed to receive data" << endl;
    }
}

int Client::readLargePayloadFromSocket(char *buffer, size_t size) {
    char smallerBuffer[BUFFER_SIZE];
    int bytesReadFromSocket = 0;
    int bytesReadCurrentIteration = 0;
    int bufferSize;

    do {
        bufferSize = determineCorrectSizeToBeCopied(size, bytesReadFromSocket);

        bytesReadCurrentIteration = read(sockfd, smallerBuffer, bufferSize);
        if (bufferSize != bytesReadCurrentIteration) {
            //cout << "Error reading current buffer in socket - should retry this part" << endl;
            break;
        }

        memcpy(buffer + bytesReadFromSocket, smallerBuffer, bufferSize);

        bytesReadFromSocket += bufferSize;
    } while(bytesReadFromSocket < size);
    return bytesReadFromSocket;
}

int Client::writeAckIntoSocket(const char *message) {
    int bytesWritenIntoSocket = write(sockfd, message, strlen(message));
    if (bytesWritenIntoSocket == -1) {
        cout << "writeAckIntoSocket: failed to write ack" << endl;
    }
    return bytesWritenIntoSocket;
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

int Client::deleteFile(char *filename)
{
    commandPacket command_packet;
    command_packet.packetType = CMD;
    command_packet.command = DELETE;
    strcpy(command_packet.additionalInfo, filename);


    cout << "[Client][Delete] Sending packet for deletion: " << command_packet.additionalInfo << endl;

    sendLargePayloadToSocket((char*)&command_packet, sizeof(struct commandPacket));
    waitForSocketAck();

    //cout << "[Client][Delete] Ack received! ";

    return 0;
}

int Client::downloadFile(char *filename) {
    commandPacket command_packet;
    command_packet.packetType = CMD;
    command_packet.command = DOWNLOAD;
    strcpy(command_packet.additionalInfo, filename);


    cout << "[Client][Download] Sending packet to ask for file: " << command_packet.additionalInfo << endl;

    sendLargePayloadToSocket((char*)&command_packet, sizeof(struct commandPacket));
    waitForSocketAck();

    char fileSizeBuffer[sizeof(long)];
    readDataFromSocket(fileSizeBuffer, sizeof(long));
    long payloadSize = *(long *)fileSizeBuffer;

    if(payloadSize <= 0){
        cout << "[Client][Download] Error: file doesn't exist in server" << endl;
    }
    else{
        writeAckIntoSocket("ack");

        char payload[payloadSize];
        readLargePayloadFromSocket(payload, payloadSize);
        writeAckIntoSocket("ack");

        stringstream pathStream;
        pathStream << "./" << filename;
        string path = pathStream.str();

        ofstream offFile(path);
        offFile.write(payload, payloadSize);
        offFile.close();

        cout << "[Client][Download] File received! ";
    }

    return 0;
}

int Client::list_server() {
    commandPacket command_packet;
    command_packet.packetType = CMD;
    command_packet.command = LIST_SERVER;

    //cout << "[Client][List server] Started" << endl;
    sendLargePayloadToSocket((char*)&command_packet, sizeof(struct commandPacket));
    waitForSocketAck();

    /*char stringSizeBuffer[sizeof(long)];
    readDataFromSocket(stringSizeBuffer, sizeof(long));
    writeAckIntoSocket("ack");
    long payloadSize = *(long *)stringSizeBuffer;

    if(payloadSize <= 0){
        cout << "[Client][List Server] Error: There are no files in the server" << endl;
    }
    else if(payloadSize > 20000){
        cout << "[Client][List Server] Error: Could not read properly" << endl;
    }
    else{
        //writeAckIntoSocket("ack");

        char payload[payloadSize];
        readLargePayloadFromSocket(payload, payloadSize);
        writeAckIntoSocket("ack");

        cout << "[Client][List Server] List Server Command: " << endl << payload << endl;
    }*/

    return 0;
}

