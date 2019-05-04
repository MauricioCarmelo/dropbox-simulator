//
// Created by vagrant on 5/4/19.
//

#ifndef DROPBOX_UTILITY_H
#define DROPBOX_UTILITY_H

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

class Utility {

    struct Instruction {
        char command_name[50];
        int command_id;
        char path[50];
        char filename[50];
    };


public:
    void waitForUserCommand();
    Instruction prepare_instruction(char line[]);
    int get_command_id(char *command);

};


#endif //DROPBOX_UTILITY_H
