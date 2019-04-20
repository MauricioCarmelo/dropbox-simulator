//
// Created by root on 17/04/19.
//

#ifndef DROPBOX_BOX_H
#define DROPBOX_BOX_H

#include "Client.h"
#include "SystemDAO.h"
#include <string.h>

#define SYNC_DIR "sync_dir"

class Box {

private:
    Client client;
    SystemDAO sda;

public:
    Box();
    ~Box() {};
    int open(char *host, int port);
    bool createSyncDir();

};

#endif //DROPBOX_BOX_H
