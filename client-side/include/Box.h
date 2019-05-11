#ifndef DROPBOX_BOX_H
#define DROPBOX_BOX_H

#include "Client.h"
#include "SystemDAO.h"
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include "../../utils/Instruction.h"

#define SYNC_DIR "sync_dir"

using namespace std;
class Instruction;
class Box {

private:
    Client client;
    SystemDAO sda;
    Client c1, c2;
    static Instruction instruction;

public:
    Box();
    ~Box() {};
    int open(char *host, int port);
    bool createSyncDir();
    int read_file(char* fileContent, std::string filePath);
    static void* th_func_monitor_console(Client client);
    static void* th_func_inotify();
};

#endif //DROPBOX_BOX_H
