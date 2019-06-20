#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../../utils/include/File.h"
#include "../../utils/include/FileManager.h"
#include "../../client-side/include/Client.h"
#include "../../utils/include/masterInclude.h"

#define BUFFER_SIZE 4

#define MAX_DEVICES 2
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

#define T1 1
#define T2 2
#define T3 3

#define DATABASE_DIR "./database"

struct Device {
    int id;
    int socket1;
    int socket2;
    int socket3;
};

struct User {
    string name;
    Device devices[MAX_DEVICES];
};

struct UserCurrentSocket {
    string userName;
    int currentDevice;
    int currentSocket;
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
void print_user_structure();

// part 2
void initiate_backup_server_structure();
int insert_server(int id, int socket);

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
    struct sockaddr_in replication_socket;


    pthread_t threads[50];
    static int determineCorrectSizeToBeRead(int totalSize, int bytesWritenInSocket);
    static int readDataFromSocket(int socketId, char *buffer, size_t size);
    static int readLargePayloadFromSocket(int socketId, char *buffer, size_t size);
    static int writeAckIntoSocket(int socketId, const char *message);

    static int sendDataToSocket(int socketId, void *data, size_t size);
    static int sendLargePayloadToSocket(int socketId, char *data, size_t totalSize);
    static void waitForSocketAck(int socketId);
    static int handle_user_controller_structure(connection_t *connection, int socket, void *arg);

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
};

#endif //DROPBOX_SERVER_H