#include "../../utils/include/masterInclude.h"
#include "../include/Client.h"
#include "../include/Box.h"
#include "../../utils/include/Utility.h"

using namespace std;

char username[USERNAME_SIZE];

int main(int argc, char *argv[]) {

    if (argc < 4) {
        cout << "[Box Main]Please use" << std::endl;
        cout << "./box <user> <address> <port>" << std::endl;
        exit(1);
    }
    bzero(username, USERNAME_SIZE);
    strcpy(username, argv[1]);
    
    char *host = (char*)malloc(strlen(argv[2]));
    strcpy(host, argv[2]);
    int port = atoi(argv[3]);

    Box box;
    box.set_username(username);
    char userFolder[FOLDER_SIZE];
    strcpy(userFolder, "./");
    strcat(userFolder, SYNC_DIR);

    box.setUserFolder(userFolder);
    box.open(host, port);

    return 0;
}