//
// Created by vagrant on 4/20/19.
//

#include "../include/Server.h"
#include <cstring>
#include <sys/stat.h>

User users[MAX_USERS];

Server::Server() = default;

int Server::createSocket(char* host, int port) { //TODO host nao é usado aqui

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cout << "[Server] ERROR opening socket" << endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        cout << "[Server] ERROR on binding" << endl;

    listen(sockfd, 5);

    clilen = sizeof(struct sockaddr_in);

    return 0;
}

void* Server::terminalThreadFunction(void *arg) {
    std::cout << "terminal thread here" << std::endl;

    char packetTypeBuffer[sizeof(uint64_t)];
    readDataFromSocket(arg, packetTypeBuffer, sizeof(uint64_t));
    writeAckIntoSocket(arg);

    char fileSizeBuffer[sizeof(uint64_t)];
    readDataFromSocket(arg, fileSizeBuffer, sizeof(uint64_t));
    writeAckIntoSocket(arg);

    char fileNameBuffer[100];
    readDataFromSocket(arg, fileNameBuffer, sizeof(fileNameBuffer));
    writeAckIntoSocket(arg);

    uint64_t payloadSize = *(int *)fileSizeBuffer;
    char payload[payloadSize];
    readLargePayloadFromSocket(arg, payload, payloadSize);
    writeAckIntoSocket(arg);

    //this block bellow is going inside the DAO
    char *prefix = (char*)malloc(200*sizeof(char));
    strcpy(prefix, "uploaded-");
    strcat(prefix, fileNameBuffer);
    ofstream offFile(prefix);
    offFile.write(payload, payloadSize);
    offFile.close();
    cout << "copiei o arquivo no path:" << prefix << endl;
}

int Server::readDataFromSocket(void *socket, char *buffer, size_t size) {
    int socketId = *(int *) socket;
    int bytesReadFromSocket = read(socketId, buffer, size);
    if (bytesReadFromSocket == -1) {
        cout << "readDataFromSocket: failed to receive data" << endl;
    }

}

int Server::readLargePayloadFromSocket(void *socket, char *buffer, size_t size) {
    int socketId = *(int *) socket;
    char smallerBuffer[BUFFER_SIZE];
    int bytesReadFromSocket = 0;
    int bytesReadCurrentIteration = 0;
    int bufferSize;

    do {
        bufferSize = determineCorrectSizeToBeRead(size, bytesReadFromSocket);

        bytesReadCurrentIteration = read(socketId, smallerBuffer, bufferSize);
        if (bufferSize != bytesReadCurrentIteration) {
            cout << "Error reading current buffer in socket - should retry this part" << endl;
        }

        memcpy(buffer + bytesReadFromSocket, smallerBuffer, bufferSize);

        bytesReadFromSocket += bufferSize;
    } while(bytesReadFromSocket < size);
    return bytesReadFromSocket;
}

int Server::determineCorrectSizeToBeRead(int payloadSize, int bytesReadFromSocket) {
    if( payloadSize - bytesReadFromSocket >= BUFFER_SIZE )
        return BUFFER_SIZE;
    else
        return payloadSize - bytesReadFromSocket;
}

int Server::writeAckIntoSocket(void *socket) {
    int socketId = *(int *) socket;
    int bytesWritenIntoSocket = write(socketId, "ack", 3);
    if (bytesWritenIntoSocket == -1) {
        cout << "writeAckIntoSocket: failed to write ack" << endl;
    }
    return bytesWritenIntoSocket;
}

void* Server::serverNotifyThreadFunction(void *arg) {

}

void* Server::iNotifyThreadFunction(void *arg) {

}

void *Server::mediatorThread(void *arg) {
    cout << "[Server] got in the mediator thread" << endl;
    connection_t connection;
    pthread_t thread;
    int socket = *(int *) arg;
    int ack;

    ack = read(socket, &connection, sizeof(struct connection));
    if(ack == -1) {
        cout << "erro no read do connection type" << endl;
    }
    ack = write(socket,"ack", 3);
    if(ack == -1) {
        cout << "erro no write ack no connection type" << endl;
    }

    // checar se o usuario esta conectado

        // se ja tiver max usuarios conectados, cancela o box

        // se usuario nao esta conectado, alocar espaco para o user, com o nome e id do device atual

        // Se o usuario esta conectado somente com um device (ou menos que o MAX_USERS), criar nova estrutura
        // de device e inserir no espaco disponivel dentro do usuario

    if(connection.type == T1) {
        // inserir socket1 na estrutura de device do usuario
        pthread_create(&thread, NULL, &Server::terminalThreadFunction, arg);

    }

    if(connection.type == T2) {
        // inserir socket2 na estrutura de device do usuario
        pthread_create(&thread, NULL, &Server::serverNotifyThreadFunction, arg);
    }

    if(connection.type == T3) {
        // inserir socket3 na estrutura de device do usuario
        pthread_create(&thread, NULL, &Server::iNotifyThreadFunction, arg);
    }

}

int Server::run() {
    int i = 0;

    while (i<50){
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
            std::cout << "[Server] ERROR on accept" << endl;

        if( pthread_create(&threads[i], NULL, &Server::mediatorThread, &newsockfd) != 0 )
            std::cout << "[Server] Failed to create thread" << endl;

        i++;
    }
}

int Server::handle_type(int s)
{
    connection_t c;

    //char buffer[7];
    //bzero(buffer, 7);
    cout << "[Server] opa3" << endl;
    n = read(s, &c, sizeof(struct connection));
    //n = read(newsockfd, buffer, 7);
    cout << "[Server] opa4" << endl;
    //std::cout << buffer << std::endl;
    cout << c.type << endl;
    if (n < 0) {
        cout << "[Server] ERROR reading from socket" << endl;
        return 1;
    }
    n = write(s, "ack", 3);

    if(c.type == T1)
        cout << "[Server] Tipo 1" << endl;
    else if(c.type == T2)
        cout << "[Server] Tipo 2" << endl;
    else
        cout << "[Server] Tipo nao definido" << endl;

    return 0;
}

int Server::countUserConnections(string user) {
    int i, count = 0;

    while (semaphore == 1){
        //do nothing
    }

    semaphore = 1;

    for (i = 0; i < 10; i++){
        if (clients[i].name == user && clients[i].isLogged)
            count++;
    }

     cout << "[Server] User " << user << "has " << count << "connections" << endl;

    semaphore = 0;

    return count;
}

void Server::createUserDirectory(const char *user) {

    struct stat st = {0};
    char diretorio[50] = "sync_dir_";
    strcat(diretorio,user);

    if (stat(diretorio, &st) != 0) {
        mkdir(diretorio, 07777);
        cout << "[Server] User: " << user << "has now a client folder" << endl;
    }
    else {
        cout << "[Server] User " << "already has a folder" << endl;
    }

}

int insert_user(std::string name)
{
    auto user = get_user(name);
    if (user != nullptr) {
        std::cout << "user already exists" << std::endl;
        return ERROR;
    }

    if (number_of_users_logged() >= MAX_USERS) {
        std::cout << "max number of users reached" << std::endl;
        return ERROR;
    }

    for(auto &user : users) {
        if(user.name == "") {
            user.name = name;
            return SUCCESS;
        }
    }
    return ERROR;
}

int remove_user(std::string name)
{
    for(auto &user : users){
        if (user.name == name) {
            if (is_device_connected(name)){
                std::cout << "canot remove user, device still connected" << std::endl;
                return ERROR;
            }
            user.name = "";
        }
    }
}

int is_device_connected(std::string name)
{
    auto user = get_user(name);
    if (user == nullptr) {
        std::cout << "user not found" << std::endl;
        return ERROR;
    }

    for (auto &device : user->devices) {
        if (device.id != -1)
            return true;
    }
    return false;
}

User* get_user(std::string name)
{
    for(auto &user : users) {
        if(user.name == name) {
            return &user;
        }
    }
    return nullptr;
}


int insert_device(std::string name, int device_id)
{

    auto user = get_user(name);
    if (user == nullptr) {
        std::cout << "user not found" << std::endl;
        return ERROR;
    }

    auto device = get_available_device(name);
    if (device == nullptr){
        std::cout << "available device not found" << std::endl;
        return ERROR;
    }

    device->id = device_id;      // insert device
    return SUCCESS;
}

Device *get_device(int device_id, std::string name)
{
    if (name != "") {               // search device for an specific user
        auto user = get_user(name);
        if (user == nullptr) {
            std::cout << "user not found" << std::endl;
            return nullptr;
        }
    }
    else  {                         // search device in the whole structure
        for (auto &user : users) {
            for (auto &device : user.devices) {
                if (device.id == device_id)
                    return &device;
            }
        }
        return nullptr;
    }
}

int number_of_devices(std::string name) {
    int n = 0;

    auto user = get_user(name);
    if (user == nullptr) {
        std::cout << "user not found" << std::endl;
        return ERROR;
    }

    for (auto &device : user->devices) {
        if (device.id != -1)
            n++;
    }
    return n;
}

int number_of_users_logged()
{
    int n = 0;

    for (auto &user : users) {
        if (user.name != "" )
            n++;
    }
    return n;
}


Device* get_available_device(std::string name)
{
    auto user = get_user(name);
    if (user == nullptr) {
        std::cout << "user not found" << std::endl;
        return nullptr;
    }

    if (number_of_devices(name) >= MAX_DEVICES) {
        std::cout << "Max number of devices reached" << std::endl;
        return nullptr;
    }

    for (auto &device : user->devices) {
        if (device.id == -1) {
            return &device;
        }
    }
}

int insert_socket(std::string name, int device_id, int socket_descriptor, int socket_type)
{
    auto device = get_device(device_id, name);
    switch (device_id){
        case T1:
            device->socket1 = socket_descriptor;
            break;
        case T2:
            device->socket2 = socket_descriptor;
            break;
        case T3:
            device->socket3 = socket_descriptor;
            break;
        default:
            std::cout << "socket type not defined" << std::endl;
            break;
    }

}

int initiate_user_controller_structure()
{
    for(auto &user : users)
    {
        user.name = "";
        for (auto &device : user.devices)
        {
            device.id = -1;
            device.socket1 = -1;
            device.socket2 = -1;
            device.socket3 = -1;
        }
    }
    return 0;
}

// test
void print_user_structure()
{
    for (auto user : users)
    {
        std::cout << user.name << std::endl;
        for (auto device : user.devices)
        {
            std::cout << "\tdevice id: " << device.id << std::endl;
            std::cout << "\tsocket 1: " << device.socket1 << std::endl;
            std::cout << "\tsocket 2: " << device.socket2 << std::endl;
            std::cout << "\tsocket 3: " << device.socket3 << std::endl;
        }
        std::cout << std::endl;
    }
}



/*
void Server::sync_server() {

}


void Server::send_file(File file, int socket) {

}
*/




