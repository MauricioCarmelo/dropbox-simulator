#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../../utils/include/File.h"
#include "../../utils/include/FileManager.h"
#include "../../client-side/include/Client.h"
#include "../../utils/include/masterInclude.h"

#define BUFFER_SIZE 4

#define MAX_USERS 5

#define MAX_SERVERS 2

#define SUCCESS 1
#define ERROR -1

#define MAX_USERS_REACHED 2
#define USER_ALREADY_CONNECTED 3

#define MAX_DEVICES_REACHED 4
#define DEVICE_ALREADY_CONNECTED 5

#define CMD 1
#define FILE 2
#define CONN 3
#define SERVERCONN 4

#define T1 1
#define T2 2
#define T3 3

#define DATABASE_DIR "./database"


struct UserCurrentSocket {
    string userName;
    int currentDevice;
    int currentSocket;
};

struct ServerArgs {
    int id;
    int socket;
};

struct BackupServer {
    int id;
    int socket;
};

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
    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    Client clients[10];
    sem_t semaphore_update_other_devices;
    sem_t semaphore_devices_updated;

    // part 2
    // socket information to secondary connect to primary
    struct hostent *primary_server;
    struct sockaddr_in primary_address;
    int primary_socket;

    int replication_socket;

    bool isPrimary;
    int id;
    string myIP;
    int myPort;
    string primaryIP, secondaryIP;
    int primaryPort, secondaryPort;
    /*string backupServerIP_1, backupServerIP_2;
    int backupServerID_1, backupServerID_2;
    int backupServerPort_1, backupServerPort_2;*/

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
    static void *deleteFileCommand(void *arg, commandPacket commandPacket);
    static void *downloadFileCommand(void *arg, commandPacket commandPacket);
    static void *listServerCommand(void *arg);
    static void *exitCommand(void *arg);

    static void* handle_one_secondary_server(void *arg);
};

#endif //DROPBOX_SERVER_H