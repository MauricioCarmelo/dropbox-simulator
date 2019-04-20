//
// Created by root on 17/04/19.
//

#ifndef DROPBOX_BOX_H
#define DROPBOX_BOX_H

#include "Client.h"

class Box {

public:
    int open(char *host, int port);

private:
    Client client;
};


#endif //DROPBOX_BOX_H
