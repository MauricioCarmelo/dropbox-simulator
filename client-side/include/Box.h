#ifndef DROPBOX_BOX_H
#define DROPBOX_BOX_H

#include "Client.h"
#include "SystemDAO.h"
#include "../../utils/include/masterInclude.h"
#include "../../utils/include/Instruction.h"

#define SYNC_DIR "sync_dir"
#define USERNAME_SIZE 100
#define FOLDER_SIZE 300

using namespace std;
class Instruction;
class Box {

private:
    Client client;
    SystemDAO sda;
    Client c1, c2, c3;
    static Instruction instruction;
public:
    Box();
    ~Box() {};
    char username[USERNAME_SIZE];
    char userFolderPath[FOLDER_SIZE];
    int device;
    char* get_username();
    void set_username(char *name);
    void setUserFolder(char *dir);
    int open(char *host, int port);
    bool createSyncDir();
    int read_file(char* fileContent, std::string filePath);
    static void* th_func_monitor_console(Client client);
    static void* th_func_inotify(Client client);
    static void* th_func_server_comm(Client client);
};

#endif //DROPBOX_BOX_H
