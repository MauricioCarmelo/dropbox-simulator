#ifndef DROPBOX_INSTRUCTION_H
#define DROPBOX_INSTRUCTION_H

#include <iostream>
#include <cstring>
#include <dirent.h>
#include <fstream>

using namespace std;

#define UPLOAD 2
#define DOWNLOAD 3
#define DELETE 4
#define LIST_SERVER 5
#define LIST_CLIENT 6
#define GET_SYNC_DIR 7
#define EXIT 8
#define INVALID_COMMAND -1

#define PATH_TO_SYNC_DIR "../cmake-build-debug/sync_dir/"

class Instruction{

private:
    char command_name[50];
    int command_id;
    char path[50];
    char filename[50];

public:
    int get_command_id();
    void prepare(char* line);
    void set_command_id();
    void print();
    void upload_file();
    void download_file();
    void delete_file();
    void list_server();
    void list_client();
    void get_sync_dir();
    void exit();
};

#endif //DROPBOX_INSTRUCTION_H
