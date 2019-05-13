
#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include "../../utils/include/masterInclude.h"
#include "../../utils/include/Instruction.h"


#define BUFFER_SIZE 4
#define CMD 1
#define FILE 2
#define CONN 3

#define T1 1
#define T2 2
#define T3 3
using namespace std;

typedef struct filePacket{
    uint64_t packetType;
    uint64_t fileSize;
    char *fileName;
    char *payload;
} filePacket;

typedef struct connection{
    uint64_t packetType;
    uint64_t socketType;
    int16_t username; // mudar pra char[]
    // mandar o id do device
} connection_t;

class Client {
private:
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    filePacket prepareFilePacket(char *filename, int size, char *fileContent);
    int sendFilePacket(filePacket file_packet);
    int determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket);
    int sendDataToSocket(void *data, size_t size);
    int sendLargePayloadToSocket(char *data, size_t totalSize);
    void waitForSocketAck();
public:
    Client();
    ~Client() {};
    Client(char *host, int port);
    int establishConnectionToHost();
    int sendFile(char *filename, int size, char *fileContent);
    int establishConnectionType(connection_t c);
    std::string name;
    bool isLogged;
};

#endif //DROPBOX_CLIENT_H