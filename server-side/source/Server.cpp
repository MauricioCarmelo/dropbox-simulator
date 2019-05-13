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

    int socket = *(int *) arg;
    int payloadSize;
    char fileBuffer[BUFFER_SIZE];
    char fileSizeBuffer[sizeof(uint16_t)];
    int bytesReadFromSocket = 0;
    int bytesReadCurrentIteration = 0;
    int bufferSize;

    ofstream offFile("out.txt");

    bytesReadFromSocket = read(socket, fileSizeBuffer, sizeof(uint16_t));
    if (bytesReadFromSocket == -1) {
        cout << "erro na leitura do size" << endl;
    }
    payloadSize = *(int *)fileSizeBuffer;
    bytesReadFromSocket = write(socket, fileSizeBuffer, sizeof(uint16_t));
    if (bytesReadFromSocket == -1) {
        cout << "erro no ack do size" << endl;
    }

    char payload[payloadSize];

    bytesReadFromSocket = 0;

    do {
        bufferSize = determineCorrectSizeToBeRead(payloadSize, bytesReadFromSocket);

        bytesReadCurrentIteration = read(socket, fileBuffer, bufferSize);
        if (bufferSize != bytesReadCurrentIteration) {
            cout << "Error reading current buffer in socket - should retry this part" << endl;
        }

        strcat(payload, fileBuffer);

        bytesReadFromSocket += bufferSize;
    } while(bytesReadFromSocket < payloadSize);

    //altough payload is a char*, it should have a file_t content, which needs to be disassembled before writing to file

    if ( fileBuffer) {
        offFile.write(payload, payloadSize);
        offFile.close();
    }

}

int Server::determineCorrectSizeToBeRead(int payloadSize, int bytesReadFromSocket) {
    if( payloadSize - bytesReadFromSocket >= BUFFER_SIZE )
        return BUFFER_SIZE;
    else
        return payloadSize - bytesReadFromSocket;
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

    // check initial configuration
    //char username = connection.username;
    char username[100];
    strcpy(username, connection.username);
    std::string user(username);
    auto device_id = connection.device;

    auto insert_user_result = insert_user(user);

    if ( insert_user_result == ERROR ) {
        ack = write(socket,"nack1", 5);
        if(ack == -1) {
            cout << "erro no write nack1 no connection type" << endl;
        }
        // TERMINAR A THREAD AQUI
    }
    else if( insert_user_result == MAX_USERS_REACHED) {
        ack = write(socket,"nack2", 5);
        if(ack == -1) {
            cout << "erro no write nack2 no connection type" << endl;
        }
        // TERMINAR A THREAD AQUI
    }
    else if( insert_user_result == USER_ALREADY_CONNECTED) {

        auto insert_device_result = insert_device(user, device_id);
        if( insert_device_result == MAX_DEVICES_REACHED ) {
            ack = write(socket,"nack3", 5);
            if(ack == -1) {
                cout << "erro no write nack3 no connection type" << endl;
            }
            // TERMINAR A THREAD AQUI
        }
        else if ( insert_device_result == DEVICE_ALREADY_CONNECTED) {

            // VER QUAL EH O T E COLOCAR NO SOCKET DO DEVICE CORRETO

            ack = write(socket,"nack4", 5);
            if(ack == -1) {
                cout << "erro no write nack4 no connection type" << endl;
            }

            // VERIFICAR SE O SOCKET REFERENTE AO TIPO DE CONEXAO ESTA CONECTADO. SE SIM, TERMINAR EXECUCAO
            // SE NAO, INSERIR O SOCKET NA ESTRUTURA DO USUARIO, NO DEVICE CORRESPONDENTE
        }
        else if (insert_device_result == SUCCESS) {
            ack = write(socket,"ack", 3);
            if(ack == -1) {
                cout << "erro no write ack no connection type" << endl;
            }
            // THREAD SEGUE EXECUCAO
        }

    }
    else if( insert_user_result == SUCCESS ) {
        auto insert_device_result = insert_device(user, device_id);
        if( insert_device_result == ERROR ) {
            ack = write(socket,"nack5", 5);
            if(ack == -1) {
                cout << "erro no write nack3 no connection type" << endl;
            }
            // REMOVE USER
            // TERMINAR A THREAD AQUI
        }
        else if (insert_user_result == SUCCESS) {
            ack = write(socket, "ack", 3);
            if(ack == -1) {
                cout << "erro no write nack3 no connection type" << endl;
            }
            // THREAD SEGUE EXECUCAO
        }
    }


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

int Server::receive_file() {

    char message[100];  // isso nao vai ficar aqui
    int file_size = 7;  // determinar o tamanho do arquivo de outra maneira
    int bytes_received;
    char buffer[BUFFER_SIZE];

    if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
        cout << "[Server] ERROR on accept" << endl;

    bytes_received = 0;
    while(bytes_received < file_size) {
        bzero(buffer, BUFFER_SIZE);

        /* read from the socket */
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            cout << "[Server] ERROR reading from socket" << endl;

        memcpy(message+bytes_received, buffer, n);
        bytes_received = bytes_received + n;
        //std::cout << "Bytes received: " << bytes_received << std::endl;
    }
    n = write(newsockfd, "ack", 3);
    if (n < 0)
        cout << "[Server] ERROR writing to socket" << endl;

    for (int i=0; i<bytes_received; i++){
        cout << message[i];
    }
    cout << endl;

    /*for (char c: message) {
        std::cout << c;
    }*/

    //close(newsockfd);
    //close(sockfd);
    return bytes_received;
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

    auto dev_aux = get_device(device_id, name);
    if (dev_aux != nullptr) {
        std::cout << "device already connected" << std::endl;
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



