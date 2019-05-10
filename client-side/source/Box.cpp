#include "../include/Box.h"
#include "../include/Client.h"
#include <string.h>
#include <stdio.h>
#include <fstream>

Box::Box()
{
}

int Box::open(char *host, int port) {
    //client = Client(host, port);
    //client.establishConnectionToHost();
    //createSyncDir();
    srand(time(0));

    // test
    //char frase[] = "frase1";
    //client.send(frase, 7);
    int username = rand()%10;

    // open connection 1
    c1 = Client(host, port);
    c1.establishConnectionToHost();
    connection_t con1;
    con1.type = T1;
    con1.username = username;
    c1.establishConnectionType(con1);

    // open connection 2
    c2 = Client(host, port);
    c2.establishConnectionToHost();
    connection_t con2;
    con2.type = T2;
    con2.username = username;
    c2.establishConnectionType(con2);

    while(1) {
        c1.establishConnectionType(con1);
        c2.establishConnectionType(con2);
    }

    std::cout << "TERMINOU" << std::endl;
}

bool Box::createSyncDir( ) {
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

int Box::read_file(char* fileContent, std::string filePath) {

    std::ifstream in(filePath);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    fileContent = new char [contents.length()+1];
    strcpy (fileContent, contents.c_str());


    return contents.length();

}


