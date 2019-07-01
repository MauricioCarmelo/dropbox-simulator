#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../../utils/include/File.h"
#include "../../utils/include/FileManager.h"
#include "../../client-side/include/Client.h"
#include "../../utils/include/masterInclude.h"

#define DATABASE_DIR "./database"


int initiate_user_controller_structure();
int insert_user(std::string name);
User* get_user(std::string name);
int insert_device(std::string name, int device_id);
int remove_device(std::string name, int device_id);
Device *get_device(int device_id, std::string name);
int number_of_devices(std::string name);
Device* get_available_device(std::string name);
int insert_socket(std::string name, int device_id, int socket_descriptor, int socket_type);
int remove_user(std::string name);
int is_device_connected(std::string name);
int number_of_users_logged();

// part 2
void initiate_backup_server_structure();
int insert_server(int id, int socket);
int clean_server_structure();


class Server {

private:
    FileManager fileManager;
    int sockfd, sock_election_send, sock_election_receive;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr, serv_addr_election;

    // part 2
    // socket information to secondary connect to primary
    struct hostent *primary_server;
    struct sockaddr_in primary_address;

    InfoMeAsPrimary infoAsPrimary;
    InfoMeAsSecondary infoAsSecondary;

    bool isPrimary;
    int id;
    string myIP;
    int myPort;
    string primaryIP, secondaryIP;
    int primaryPort, secondaryPort;

    pthread_t threads[50];
    static int determineCorrectSizeToBeRead(int totalSize, int bytesWritenInSocket);
    static int readDataFromSocket(int socketId, char *buffer, size_t size);
    static int readLargePayloadFromSocket(int socketId, char *buffer, size_t size);
    static int writeAckIntoSocket(int socketId, const char *message);

    static int sendDataToSocket(int socketId, void *data, size_t size);
    static int sendLargePayloadToSocket(int socketId, char *data, size_t totalSize);
    static void waitForSocketAck(int socketId);
    static int handle_user_controller_structure(connection_t *connection, int socket, void *arg);

    void getBackupServersIPs();
    void second_server_processing(int primary_socket);

    int sendLargePayload(char *data, size_t totalSize, int s);
    int determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket);
    int sendDataToSocket(void *data, size_t size, int s);

    int primary_set_info(int id, int port, int size_ip, char *ip); // size_ip: tamanho do array de char *ip
    int secondary_set_info(int id, int port, int size_ip, char *ip);
    int secondary_set_info_from_primary(int port, int size_ip, char *ip);
    int secondary_set_info_from_secondary(int port, int size_ip, char *ip);


public:
    Server();
    ~Server() {};
    int createSocket(char* host, int port);
    static void createUserDirectory(const char *user);
    int run();
    int handle_type(int s);
    static void *mediatorThread(void *arg);
    static void *terminalThreadFunction(void *arg);
    static void *iNotifyThreadFunction(void *arg);
    static void *serverNotifyThreadFunction(void *arg);
    static void *uploadFileCommand(void *arg);
    static void *receiveFileUploadedFromPrimary(int primarySocket, commandPacket commandPacket);
    static void *deleteFileCommand(void *arg, commandPacket commandPacket);
    static void *deleteFileDeletedInPrimary(int primarySocket, commandPacket commandPacket);
    static void *insertUserConnectedInPrimary(int primarySocket);
    static void *receiveExitFromPrimary(int primarySocket);
    static void *downloadFileCommand(void *arg, commandPacket commandPacket);
    static void *listServerCommand(void *arg);
    static void *exitCommand(void *arg);
    static void *sendHeartbeatThreadFunction(void *arg);
    static void* receiveHeartbeatThreadFunction(void *arg);

    static void* handle_one_secondary_server(void *arg);

};

#endif //DROPBOX_SERVER_H