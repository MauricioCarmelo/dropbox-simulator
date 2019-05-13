//
// Created by vagrant on 4/20/19.
//

#ifndef DROPBOX_SERVER_H
#define DROPBOX_SERVER_H

#include "../../client-side/include/File.h"
#include "../../client-side/include/Client.h"
#include "../../utils/include/masterInclude.h"
#include <string>
#include <iostream>

#define BUFFER_SIZE 4   // fazer um buffer maior, 256

#define MAX_DEVICES 2
#define MAX_USERS 5

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

struct Device {
    int id;
    int socket1;
    int socket2; // escolher um nome melhor pros sockets
    int socket3;
};

struct User {
    string name;
    Device devices[MAX_DEVICES];
};

int initiate_user_controller_structure();
int insert_user(std::string name);
User* get_user(std::string name);
int insert_device(std::string name, int device_id);
Device *get_device(int device_id, std::string name);
int number_of_devices(std::string name);
Device* get_available_device(std::string name);
int insert_socket(std::string name, int device_id, int socket_descriptor, int socket_type);
int remove_user(std::string name);
int is_device_connected(std::string name);
int number_of_users_logged();

// test
void print_user_structure();


class Server {

private:

    // We will need arrays of sockets here in order to handle multiple users

    int sockfd, newsockfd, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    Client clients[10];
    int semaphore = 0; // we should need this in the future
    pthread_t threads[50];
    //static User users[MAX_USERS];
    static int determineCorrectSizeToBeRead(int totalSize, int bytesWritenInSocket);
    static int readDataFromSocket(void *socket, char *buffer, size_t size);
    static int readLargePayloadFromSocket(void *socket, char *buffer, size_t size);
    static int writeAckIntoSocket(void *socket);

public:

    Server();
    ~Server() {};
    //Server(const File &fileInfo, const Client &client);
    int createSocket(char* host, int port);
    int receive_file();
    int countUserConnections(std::string user); //counts how many connections a user has
    void createUserDirectory(const char *user);
    //int receive_file_stream(char *stream);
    int run();
    int handle_type(int s);
    static void *mediatorThread(void *arg);
    static void *terminalThreadFunction(void *arg);
    static void *iNotifyThreadFunction(void *arg);
    static void *serverNotifyThreadFunction(void *arg);
    static void *uploadFileCommandThread(void *arg);
/*
    void sync_server();                 //TODO Syncs server with directory "sync_dir_<nomeusuário>" and client

    void send_file(File file, int socket);
    const File &getFileInfo() const;
    void setFileInfo(const File &fileInfo);
    const Client &getClient() const;
    void setClient(const Client &client);
    // TODO Sends file to client - filename.ext
    */
};

#endif //DROPBOX_SERVER_H