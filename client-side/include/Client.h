#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include "../../utils/include/masterInclude.h"
#include "../../utils/include/Instruction.h"



using namespace std;

class Client {

private:
    struct sockaddr_in serv_addr;
    struct hostent *server;
    filePacket prepareFilePacket(char *filename, int size, char *fileContent);
    int sendFilePacket(filePacket file_packet);
    int determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket);

public:
    Client();
    ~Client() {};
    int sockfd;
    Client(char *host, int port);
    int establishConnectionToHost();
    int sendFile(char *filename, int size, char *fileContent);
    int deleteFile(char* filename);
    int list_server();
    int downloadFile(char *filename);
    int establishConnectionType(connection_t c);
    int sendExitCommand();
    std::string name;
    bool isLogged;
    int sendDataToSocket(void *data, size_t size);
    int sendLargePayloadToSocket(char *data, size_t totalSize);
    void waitForSocketAck();
    int readDataFromSocket(char *buffer, size_t size);
    int readLargePayloadFromSocket(char *buffer, size_t size);
    int writeAckIntoSocket(const char *message);
};

#endif //DROPBOX_CLIENT_H