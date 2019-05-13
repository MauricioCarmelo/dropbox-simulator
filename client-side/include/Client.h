
#ifndef DROPBOX_CLIENT_H
#define DROPBOX_CLIENT_H

#include "../../utils/include/masterInclude.h"
#include "../../utils/include/Instruction.h"


#define BUFFER_SIZE 4
#define CMD 1
#define DATA 2

#define T1 1
#define T2 2
#define T3 3
//typedef struct file_t file_t;
using namespace std;

typedef struct packet{
    char *fileName;
    uint64_t length;
    char *payload;
} packet;

typedef struct connection{
    int16_t type;
    int16_t username; // mudar pra char[]
    // mandar o id do device
} connection_t;

class Client {
private:
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket);
public:
    Client();
    ~Client() {};
    Client(char *host, int port);
    int establishConnectionToHost();
    int send(char *filename, int size, char *fileContent);

    int establishConnectionType(connection_t c);

    std::string name;
    bool isLogged;
    packet prepare_data_packet(char *filename, int size, char *fileContent);
    int send_data_packet(packet data_packet);
};

#endif //DROPBOX_CLIENT_H