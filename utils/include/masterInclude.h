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

#endif //DROPBOX_MASTERINCLUDE_H

