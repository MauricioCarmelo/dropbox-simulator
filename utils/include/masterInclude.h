#ifndef DROPBOX_MASTERINCLUDE_H
#define DROPBOX_MASTERINCLUDE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <sys/inotify.h>
#include <fstream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <dirent.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sstream>
#include <arpa/inet.h>
#include <string>
#include <semaphore.h>

using namespace std;

#define MAX_DEVICES 2

#define BUFFER_SIZE 4
#define CMD 1
#define FILE 2
#define CONN 3
#define PROP 4

#define T1 1
#define T2 2
#define T3 3

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

#define HEARTBEAT_EMPTY 1
#define HEARTBEAT_DATA 2

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

typedef struct filePacket{
    uint64_t packetType;
    uint64_t fileSize;
    char *fileName;
    char *payload;
} filePacket;

typedef struct commandPacket {
    uint64_t packetType;
    uint64_t command;
    char additionalInfo[100];
} commandPacket;

typedef struct connection{
    uint64_t packetType;
    uint64_t socketType;
    int device;
    char username[100];
} connection_t;

typedef struct userPacket{
    uint64_t packetType;
    User userStruct;
} userPacket;

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

struct PropagationPackage{
    int type;
};

#endif //DROPBOX_MASTERINCLUDE_H

