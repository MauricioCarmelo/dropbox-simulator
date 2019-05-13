#include "../include/Client.h"
#include "../../utils/include/Instruction.h"

Client::Client() {}


Client::Client(char *host, int port)
{
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "[Client] ERROR, no such host\n");
        exit(0);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        cout << " [Client] ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
}

int Client::establishConnectionToHost()
{
    if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        cout << " [Client] ERROR connecting" << std::endl;

    return 0;
}

int Client::establishConnectionType(connection_t c)
{
    int n;
    char buffer[100];
    //char frase[] = "frase1";
    //memcpy(buffer, frase, 7);
    n = write(sockfd, &c, sizeof(struct connection));
    n = read(sockfd, buffer, 100);


    //n = write(sockfd, buffer, 7);


    if (n < 0)
        cout << " [Client] ERROR reading from socket on  establishConnectionType()" << std::endl;

    //close(sockfd);
    return 0;
}

int Client::send(char *filename, int size, char *fileContent)
{

    /* send command to warn server that a data packet will be sent */
    // send_command_packet();

    /* send the packet */
    packet data_packet = prepare_data_packet(filename, size, fileContent);
    send_data_packet(data_packet);

    return 0;
}

packet Client::prepare_data_packet(char *filename, int size, char *fileContent)
{
    packet data_packet;

    data_packet.length = size;
    data_packet.payload = (char*)malloc(size);
    memcpy(data_packet.fileName, filename, strlen(filename));
    memcpy(data_packet.payload, fileContent, size);

    return data_packet;
}

int Client::send_data_packet(packet data_packet)
{
    int returnFromWrittenSize;
    int ackReturn;
    char ackBuffer[sizeof(uint64_t)];
    char buffer[BUFFER_SIZE];
    int bytesCopiedFromPayload = 0;
    int bytesWritenInSocket = 0;
    int bytesWritenInCurrentIteration = 0;
    int totalSize = data_packet.length;
    int bufferSize;

    returnFromWrittenSize = write(sockfd, &data_packet.length, sizeof(uint64_t));
    if (returnFromWrittenSize != sizeof(uint64_t)) {
        cout << "nao to enviando o length direito" << endl;
    }
    ackReturn = read(sockfd, ackBuffer, sizeof(uint64_t));
    if (ackReturn == -1) {
        cout << "nao recebi ack direito" << endl;
    }

    do {
        bufferSize = determineCorrectSizeToBeCopied(totalSize, bytesWritenInSocket);

        memcpy(buffer, data_packet.payload + bytesCopiedFromPayload, bufferSize);
        bytesCopiedFromPayload += bufferSize;

        bytesWritenInCurrentIteration = write(sockfd, buffer, bufferSize);
        if (bufferSize != bytesWritenInCurrentIteration) {
            cout << "Error writing current buffer in socket - should retry this part" << endl;
        }

        bytesWritenInSocket += bytesWritenInCurrentIteration;

    } while (bytesWritenInSocket < totalSize);

    ackReturn = read(sockfd, ackBuffer, sizeof(uint64_t));
    if (ackReturn == -1) {
        cout << "nao recebi ack direito" << endl;
    }

    close(sockfd); // Socket won't close here in production mode*/
    return 0;
}

int Client::determineCorrectSizeToBeCopied(int totalSize, int bytesWritenInSocket) {
    if( totalSize - bytesWritenInSocket >= BUFFER_SIZE )
        return BUFFER_SIZE;
    else
        return totalSize - bytesWritenInSocket;
}

