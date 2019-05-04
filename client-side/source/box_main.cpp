#include <iostream>
#include <cstring>
#include <thread>
#include "../include/Client.h"
#include "../include/Box.h"
#include "../../utils/Utility.h"

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

    thread th_monitor_console(waitForUserCommand);

    th_monitor_console.join();

    //Box box;
    //box.open(host, port);

    return 0;
}