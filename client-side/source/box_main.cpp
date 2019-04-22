#include <iostream>
#include <cstring>
#include "../include/Client.h"
#include "../include/Box.h"

#define USERNAME_SIZE 25

using namespace std;

char username[USERNAME_SIZE];


int main(int argc, char *argv[]) {

    if (argc < 4) {
        std::cout << "Please use" << std::endl;
        std:: cout << "./box <user> <address> <port>" << std::endl;
        exit(1);
    }

    strcpy(username, argv[1]);
    char *host = (char*)malloc(strlen(argv[2]));
    strcpy(host, argv[2]);
    int port = atoi(argv[3]);

    Box box;
    box.open(host, port);

    return 0;
}