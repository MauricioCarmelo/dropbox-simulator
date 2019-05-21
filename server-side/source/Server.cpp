//
// Created by vagrant on 4/20/19.
//

#include "../include/Server.h"
#include <cstring>
#include <sys/stat.h>

User users[MAX_USERS];
sem_t mutex_user_structure;

Server::Server() = default;

int Server::createSocket(char* host, int port) { //TODO host nao é usado aqui

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cout << "[Server] ERROR opening socket" << endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    //serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_addr.s_addr = inet_addr(host);
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        cout << "[Server] ERROR on binding" << endl;

    listen(sockfd, 5);

    clilen = sizeof(struct sockaddr_in);

    return 0;
}

void* Server::uploadFileCommand(void *arg) {
    cout << "Upload File Thread" << endl;

    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    int socket = userCurrentSocket->currentSocket;
    string username = userCurrentSocket->userName;
    int device = userCurrentSocket->currentDevice;

    char packetTypeBuffer[sizeof(uint64_t)];
    readDataFromSocket(socket, packetTypeBuffer, sizeof(uint64_t));
    writeAckIntoSocket(socket, "ack");

    char fileSizeBuffer[sizeof(uint64_t)];
    readDataFromSocket(socket, fileSizeBuffer, sizeof(uint64_t));
    writeAckIntoSocket(socket, "ack");

    char fileNameBuffer[100];
    for(int i = 0; i < 99; i++)
        fileNameBuffer[i] = '\0'; // avoid bug at filename
    readDataFromSocket(socket, fileNameBuffer, sizeof(fileNameBuffer));
    writeAckIntoSocket(socket, "ack");

    uint64_t payloadSize = *(int *)fileSizeBuffer;
    char payload[payloadSize];
    readLargePayloadFromSocket(socket, payload, payloadSize);
    writeAckIntoSocket(socket, "ack");

    stringstream completePathStream;
    completePathStream << DATABASE_DIR << "/" << username << "/" << fileNameBuffer;
    string completePath = completePathStream.str();

    ofstream offFile(completePath);
    offFile.write(payload, payloadSize);
    offFile.close();
    cout << " [SERVER] Arquivo copiado no path:" << completePath << endl;


    auto user_pointer = get_user(username);
    for(auto& device_index : user_pointer->devices) {
        if (device_index.id != -1) {
            int socketForServerComm = device_index.socket3;
            commandPacket command;
            command.command = UPLOAD;
            command.packetType = CMD;
            strcpy(command.additionalInfo, fileNameBuffer);
            sendLargePayloadToSocket(socketForServerComm, (char*)&command, sizeof(struct commandPacket));
            sendDataToSocket(socketForServerComm, &payloadSize, sizeof(payloadSize));
            sendLargePayloadToSocket(socketForServerComm, payload, payloadSize);
        }
    }
}

void* Server::downloadFileCommand(void *arg, commandPacket command) {
    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    int socket = userCurrentSocket->currentSocket;
    string user = userCurrentSocket->userName;

    stringstream streamPath;
    streamPath << DATABASE_DIR << "/" << user << "/" << command.additionalInfo;
    string path = streamPath.str();

    ifstream wantedFile(path, ifstream::binary);
    if (wantedFile) {
        wantedFile.seekg(0, wantedFile.end);
        long length = wantedFile.tellg();
        wantedFile.seekg(0, wantedFile.beg);

        char* fileContent = (char*)malloc(length);

        cout << "[Server] Reading " << streamPath.str() << endl;
        wantedFile.read(fileContent, length);

        if(fileContent){
            cout << "[Server] Whole file read successfully, " << length << " bytes read" << endl;

            sendDataToSocket(socket, &length, sizeof(long));
            waitForSocketAck(socket);

            sendLargePayloadToSocket(socket, fileContent, length);
            waitForSocketAck(socket);
        }
        else
            cout << "[Server] Error: couldn't read whole file" << endl;

        wantedFile.close();

        delete[] fileContent;
    }
    else{
        cout << "[Server] Error: couldn't find file " << command.additionalInfo << endl;
        char* not_downloaded_message = (char*)malloc(sizeof("ndown"));
        sendDataToSocket(socket, not_downloaded_message, sizeof("ndown"));
    }

}

void* Server::deleteFileCommand(void *arg, commandPacket command) {

    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    int socket = userCurrentSocket->currentSocket;
    string userName = userCurrentSocket->userName;

    stringstream filepath;
    filepath << "./database/" << userName << "/" << command.additionalInfo;

    string filepathstring = filepath.str();

    int remove_result = remove(filepathstring.c_str());
    if(remove_result != 0)
        perror("Error deleting file");
    else
        cout << "[Instruction] File " << command.additionalInfo << "deleted succesfully" << endl;

    auto user_pointer = get_user(userName);
    for(auto& device_index : user_pointer->devices) {
        if(device_index.id != -1) {
            int socketForServerComm = device_index.socket3;
            sendLargePayloadToSocket(socketForServerComm, (char*)&command, sizeof(struct commandPacket));
        }
    }

}

void* Server::listServerCommand(void *arg) {

    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    string userName = userCurrentSocket->userName;
    int socket = userCurrentSocket->currentSocket;

    stringstream filepath;
    filepath << "./database/" << userName;
    string filepathstring = filepath.str();
    stringstream dataInStringStream;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (filepathstring.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){
                struct stat file_status;
                if(stat(filepathstring.c_str(), &file_status) == 0){
                    timespec modification_time = file_status.st_mtim;
                    timespec access_time = file_status.st_atim;
                    timespec changed_time = file_status.st_ctim;
                    char* formatted_time = (char*)malloc(32);

                    dataInStringStream << "File: " << ent->d_name << "\n";

                    format_from_timespec_to_string(formatted_time, modification_time);
                    dataInStringStream << "\tModification time: " << formatted_time << "\n";

                    format_from_timespec_to_string(formatted_time, access_time);
                    dataInStringStream << "\tAccess time: " << formatted_time << "\n";

                    format_from_timespec_to_string(formatted_time, changed_time);
                    dataInStringStream << "\tChange time: " << formatted_time << "\n";

                }
            }
        }
        closedir (dir);
        string dataInString = dataInStringStream.str();

        long lenght = dataInString.length();

        sendDataToSocket(socket, &lenght, sizeof(long));
        waitForSocketAck(socket);

        sendLargePayloadToSocket(socket, const_cast<char *>(dataInString.c_str()), dataInString.length());
        waitForSocketAck(socket);
    } else {
        /* could not open directory */
        cout << "[List Server] Could not open directory";
    }

}

void* Server::exitCommand(void *arg) {
    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    string userName = userCurrentSocket->userName;
    int socket = userCurrentSocket->currentSocket;
    int device = userCurrentSocket->currentDevice;

    if (remove_device(userName, device) == SUCCESS) {
        if ( !is_device_connected(userName) ) {
            remove_user(userName);                  // remove user
        }
    }
    else{
        std::cout << "[SERVER]: User device still connected" << std::endl;
    }
}

void* Server::terminalThreadFunction(void *arg) {
    std::cout << "terminal thread here" << std::endl;

    commandPacket command;
    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    int socket = userCurrentSocket->currentSocket;

    int loopControl = 1;

    while(loopControl) {
        cout << "ready to receive command" << endl;
        readLargePayloadFromSocket(socket, (char*)&command, sizeof(struct commandPacket));
        writeAckIntoSocket(socket, "ack");

        switch (command.command){
            case UPLOAD:
                uploadFileCommand(arg);
                break;
            case DOWNLOAD:
                downloadFileCommand(arg, command);
                break;
            case DELETE:
                deleteFileCommand(arg, command);
                break;
            case LIST_SERVER:
                listServerCommand(arg);
                break;
            case EXIT:
                sem_wait(&mutex_user_structure);
                exitCommand(arg);
                sem_post(&mutex_user_structure);
                pthread_exit(arg);
            default:
                std::cout << "Command Invalid" << std::endl;
                break;
        }
    }

}

void* Server::iNotifyThreadFunction(void *arg) {
    std::cout << "inotify thread here" << std::endl;

    commandPacket command;
    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)arg;
    int socket = userCurrentSocket->currentSocket;

    int loopControl = 1;

    while(loopControl) {
        cout << "ready to receive command" << endl;
        readLargePayloadFromSocket(socket, (char*)&command, sizeof(struct commandPacket));
        writeAckIntoSocket(socket, "ack");

        switch (command.command){
            case UPLOAD:
                uploadFileCommand(arg);
                break;
            case DOWNLOAD:
                downloadFileCommand(arg, command);
                break;
            case DELETE:
                deleteFileCommand(arg, command);
                break;
            case LIST_SERVER:
                listServerCommand(arg);
                break;
            case EXIT:
                sem_wait(&mutex_user_structure);
                exitCommand(arg);
                sem_post(&mutex_user_structure);
                pthread_exit(arg);
            default:
                std::cout << "Command Invalid" << std::endl;
                break;
        }
    }
}

int Server::writeAckIntoSocket(int socketId, const char *message) {
    int bytesWritenIntoSocket = write(socketId, message, strlen(message));
    if (bytesWritenIntoSocket == -1) {
        cout << "writeAckIntoSocket: failed to write ack" << endl;
    }
    return bytesWritenIntoSocket;
}

int Server::readDataFromSocket(int socketId, char *buffer, size_t size) {
    int bytesReadFromSocket = read(socketId, buffer, size);
    if (bytesReadFromSocket == -1) {
        cout << "readDataFromSocket: failed to receive data" << endl;
    }
}

int Server::readLargePayloadFromSocket(int socketId, char *buffer, size_t size) {
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

int Server::sendDataToSocket(int socketId, void *data, size_t size) {
    int bytesSocketReceived = write(socketId, data, size);
    if (bytesSocketReceived != size) {
        cout << "sendDataToSocket: Failed to send data to socket" << endl;
    }
    return bytesSocketReceived;
}

int Server::sendLargePayloadToSocket(int socketId, char *data, size_t totalSize) {
    char buffer[BUFFER_SIZE];
    int bytesCopiedFromPayload = 0;
    int bytesWritenInSocket = 0;
    int bytesWritenInCurrentIteration = 0;
    int bufferSize;
    do {
        bufferSize = determineCorrectSizeToBeRead(totalSize, bytesWritenInSocket);

        memcpy(buffer, data + bytesCopiedFromPayload, bufferSize);
        bytesCopiedFromPayload += bufferSize;

        bytesWritenInCurrentIteration = sendDataToSocket(socketId, buffer, bufferSize);
        if (bufferSize != bytesWritenInCurrentIteration) {
            cout << "Client.sendFilePacket: Error writing current buffer in socket" << endl;
        }

        bytesWritenInSocket += bytesWritenInCurrentIteration;

    } while (bytesWritenInSocket < totalSize);
    return bytesWritenInSocket;
}

void Server::waitForSocketAck(int socketId) {
    char ackBuffer[sizeof(uint64_t)];
    int ackReturn = read(socketId, ackBuffer, sizeof(uint64_t));
    if (ackReturn == -1) {
        cout << "waitForSocketAck: Failed to receive ack" << endl;
    }
}

void* Server::serverNotifyThreadFunction(void *arg) {

}

int Server::handle_user_controller_structure(connection_t *connection, int socket, void *arg) {

    char username[100];
    strcpy(username, connection->username);
    std::string user(username);
    auto device_id = connection->device;

    auto insert_user_result = insert_user(user);

    if ( insert_user_result == ERROR ) {
        writeAckIntoSocket(socket, "nack1");
        pthread_exit(arg);
        // THREAD TERMINA AQUI
    }
    else if( insert_user_result == MAX_USERS_REACHED) {
        //write(socket,"nack2", 5);
        writeAckIntoSocket(socket, "nack2");
        pthread_exit(arg);
        // THREAD TERMINA AQUI
    }
    else if( insert_user_result == USER_ALREADY_CONNECTED) {

        auto insert_device_result = insert_device(user, device_id);
        if( insert_device_result == MAX_DEVICES_REACHED ) {
            writeAckIntoSocket(socket, "nack3");
            pthread_exit(arg);
            // THREAD TERMINA AQUI
        }
        else if ( insert_device_result == DEVICE_ALREADY_CONNECTED) {
            insert_socket(user, device_id, socket, connection->socketType);
            writeAckIntoSocket(socket, "ack");
            // THREAD SEGUE EXECUCAO
        }
        else if (insert_device_result == SUCCESS) {
            insert_socket(user, device_id, socket, connection->socketType);
            writeAckIntoSocket(socket, "ack");
            // THREAD SEGUE EXECUCAO
        }

    }
    else if( insert_user_result == SUCCESS ) {
        auto insert_device_result = insert_device(user, device_id);
        if( insert_device_result == ERROR ) {
            writeAckIntoSocket(socket, "nack5");
            if (is_device_connected(user) == false)
                remove_user(user);
            pthread_exit(arg);
        }
        else if (insert_user_result == SUCCESS) {
            insert_socket(user, device_id, socket, connection->socketType);
            writeAckIntoSocket(socket, "ack");
            // THREAD SEGUE EXECUCAO
        }
    }
}

void *Server::mediatorThread(void *arg) {
    cout << "[Server] got in the mediator thread" << endl;
    connection_t connection;
    pthread_t thread;
    int socket = *(int *) arg;

    readLargePayloadFromSocket(socket, (char*)&connection, sizeof(struct connection));


    sem_wait(&mutex_user_structure);     // lock na secao critica
    handle_user_controller_structure(&connection, socket, arg);
    sem_post(&mutex_user_structure);     // liberar secao critica

    createUserDirectory(connection.username);
    UserCurrentSocket *userCurrentSocket = (UserCurrentSocket*)malloc(sizeof(UserCurrentSocket));
    userCurrentSocket->userName = connection.username;
    userCurrentSocket->currentDevice = connection.device;
    userCurrentSocket->currentSocket = socket; //test before conversion

    if (connection.packetType == CONN) {
        if(connection.socketType== T1) {
            // inserir socket1 na estrutura de device do usuario
            pthread_create(&thread, NULL, &Server::terminalThreadFunction, userCurrentSocket);
        }

        if(connection.socketType== T2) {
            // inserir socket2 na estrutura de device do usuario
            pthread_create(&thread, NULL, &Server::iNotifyThreadFunction, userCurrentSocket);
        }

        if(connection.socketType== T3) {
            // inserir socket3 na estrutura de device do usuario
            //pthread_create(&thread, NULL, &Server::serverNotifyThreadFunction, arg); does not need a thread for him, just need him available
        }
    } else {
        cout << "Expected CONN packet in mediator and received something else" << endl;
        pthread_exit(arg);
    }
}

int Server::run() {
    int i = 0;
    fileManager.createDir(DATABASE_DIR);
    initiate_user_controller_structure();

    sem_init(&mutex_user_structure, 0, 1);
    //sem_init(&semaphore_update_other_devices, 0, 0);
    //sem_init(&semaphore_devices_updated, 0, 0);

    while (i<50){
        int *newsockfd_address;
        newsockfd_address = (int*)malloc(sizeof(int));

        if ((*newsockfd_address = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1)
            std::cout << "[Server] ERROR on accept" << endl;

        if( pthread_create(&threads[i], NULL, &Server::mediatorThread, newsockfd_address) != 0 )
            std::cout << "[Server] Failed to create thread" << endl;

        i++;
    }
}

void Server::createUserDirectory(const char *user) {

    struct stat st = {0};
    char diretorio[50] = DATABASE_DIR;
    strcat(diretorio,"/");
    strcat(diretorio, user);

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
        return USER_ALREADY_CONNECTED;
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
        return DEVICE_ALREADY_CONNECTED;
    }

    auto device = get_available_device(name);
    if (device == nullptr){
        std::cout << "available device not found" << std::endl;
        return MAX_DEVICES_REACHED;
    }

    device->id = device_id;      // insert device
    return SUCCESS;
}

int remove_device(std::string name, int device_id)
{
    auto user = get_user(name);
    if (user == nullptr) {
        std::cout << "user not found" << std::endl;
        return ERROR;
    }

    auto dev_aux = get_device(device_id, name);
    if (dev_aux == nullptr) {
        std::cout << "device not found" << std::endl;
        return ERROR;
    }

    dev_aux->id = -1;
    dev_aux->socket1 = -1;
    dev_aux->socket2 = -1;
    dev_aux->socket3 = -1;
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
        for (auto &device : user->devices) {
            if (device.id == device_id) {
                return &device;
            }
        }
        return nullptr;
    }
    /*else  {                         // search device in the whole structure
        for (auto &user : users) {
            for (auto &device : user.devices) {
                if (device.id == device_id)
                    return &device;
            }
        }
        return nullptr;
    }*/
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
    switch (socket_type){
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




