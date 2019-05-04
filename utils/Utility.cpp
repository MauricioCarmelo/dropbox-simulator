//
// Created by vagrant on 5/4/19.
//

#include "Utility.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

void waitForUserCommand() {
    char line[100];
    Instruction instruction;

    while(instruction.command_id != EXIT)
    {
        scanf("%[^\n]", line);
        getchar();

        instruction = prepare_instruction(line);
        print_instruction(instruction);
    }
}

Instruction prepare_instruction(char *line) {
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

int get_command_id(char *command) {
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

void print_instruction(Instruction inst){
    std::cout << "Você digitou o comando " << inst.command_name << " cujo ID eh " << inst.command_id;
    if(strcmp(inst.filename, "") != 0)
        std::cout << " com o arquivo " << inst.path << inst.filename << std::endl;
    else
        std::cout << "\n";
}