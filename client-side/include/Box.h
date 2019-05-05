//
// Created by root on 17/04/19.
//

#ifndef DROPBOX_BOX_H
#define DROPBOX_BOX_H

#include "Client.h"
#include "SystemDAO.h"
#include <string.h>

#define SYNC_DIR "sync_dir"

#define UPLOAD 2
#define DOWNLOAD 3
#define DELETE 4
#define LIST_SERVER 5
#define LIST_CLIENT 6
#define GET_SYNC_DIR 7
#define EXIT 8
#define INVALID_COMMAND -1

/*struct Instruction {
    char command_name[50];
    int command_id;
    char path[50];
    char filename[50];
};*/

class Box {

private:
    Client client;
    SystemDAO sda;
public:
    Box();
    ~Box() {};
    int open(char *host, int port);
    bool createSyncDir();
    int read_file(char *file, char filename);
};

#endif //DROPBOX_BOX_H
