//
// Created by vagrant on 5/4/19.
//

#include "Utility.h"
#include <string.h>
#include <stdio.h>

void Utility::waitForUserCommand() {
    char line[100];
    while(1)
    {
        scanf("%[^\n]", line);
        getchar();

        Instruction instruction;
        instruction = prepare_instruction(line);

        break; // remover isso depois
    }
}

Utility::Instruction Utility::prepare_instruction(char *line) {
    Instruction inst;
    auto command_name = strtok(line, " ");
    strcpy(inst.command_name, command_name);
    inst.command_id = get_command_id(inst.command_name);

    switch (inst.command_id)
    {
        case UPLOAD: {
            line = strtok(NULL, "");
            char *pointer_to_name = strrchr(line, '/');

            strncpy(inst.path, line, pointer_to_name - line + 1);
            strcpy(inst.filename, pointer_to_name + 1);
            break;
        }
        case DOWNLOAD: {
            line = strtok(NULL, "");
            strcpy(inst.filename, line);
            strcpy(inst.path, "");
            break;
        }
        default: {
            strcpy(inst.path, "");
            strcpy(inst.filename, "");
            break;
        }
    }
    return inst;
}

int Utility::get_command_id(char *command) {
    if ( strcmp(command, "upload") ==  0 )
        return UPLOAD;
    else if ( strcmp(command, "download") ==  0 )
        return DOWNLOAD;
    else if ( strcmp(command, "delete") ==  0 )
        return DELETE;
    else if ( strcmp(command, "list_server") ==  0 )
        return LIST_SERVER;
    else if ( strcmp(command, "list_client") ==  0 )
        return LIST_CLIENT;
    else if ( strcmp(command, "get_sync_dir") ==  0 )
        return GET_SYNC_DIR;
    else if( strcmp(command, "exit") == 0 )
        return EXIT;
    else
        return INVALID_COMMAND;
}
