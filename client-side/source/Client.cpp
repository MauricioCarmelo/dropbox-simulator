#include "../include/Client.h"

Client::Client() {}


Client::Client(char *host, int port)
{
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
        std::cout << "ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
}

int Client::establishConnectionToHost()
{
    if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 )
        std::cout << "ERROR connecting" << std::endl;

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
        std::cout << "ERROR reading from socket on  establishConnectionType()" << std::endl;

    //close(sockfd);
    return 0;
}

int Client::send(char *data, int size)
{

    /* send command to warn server that a data packet will be sent */
    // send_command_packet();

    /* send the packet */
    packet data_packet = prepare_data_packet(data, size);
    send_data_packet(data_packet);

    return 0;
}

packet Client::prepare_data_packet(char *data, int size)
{
    packet data_packet;

    data_packet.type = DATA;
    data_packet.length = size;
    data_packet.payload = (char*)malloc(size);
    memcpy(data_packet.payload, data, size);

    return data_packet;
}

int Client::send_data_packet(packet data_packet)
{
    char buffer[BUFFER_SIZE];
    int n;
    int bytes_amount;
    int buffer_size;
    int size = data_packet.length;
    int sent_bytes = 0;
    do {
        if( size-sent_bytes >= BUFFER_SIZE )
            buffer_size = BUFFER_SIZE;
        else
            buffer_size = size - sent_bytes;

        memcpy(buffer, data_packet.payload+sent_bytes, buffer_size);
        //n = write(sockfd, buffer, sizeof(buffer) - size + buffer_size), onde buffer eh a struct
        n = write(sockfd, buffer, buffer_size);
        sent_bytes += n;
    } while ( sent_bytes < size);

    n = read(sockfd, buffer, 256);
    if (n < 0)
        printf("ERROR reading from socket\n");
    printf("%s\n", buffer);

    close(sockfd); // Socket won't close here in production mode
    return 0;
}

