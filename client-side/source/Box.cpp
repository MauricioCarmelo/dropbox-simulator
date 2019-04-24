#include "../include/Box.h"
#include "../include/Client.h"
#include <string.h>

Box::Box()
{
}

int Box::open(char *host, int port) {
    client = Client(host, port);
    client.establishConnectionToHost();
    createSyncDir();
    char frase[] = "frase1";
    client.send(frase, 7);
}

bool Box::createSyncDir( )
{
    std::string dir_path;
    dir_path = "./";
    dir_path += SYNC_DIR;
    // convert string to array of char
    char path[dir_path.length() + 1];
    strcpy(path, dir_path.c_str());
    if ( sda.createDir(path) )
        return true;
    return false;
}