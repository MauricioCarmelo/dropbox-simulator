#include "../include/Box.h"
#include "../include/Client.h"
#include <string.h>
#include <stdio.h>

Box::Box()
{
}

int Box::open(char *host, int port) {
    client = Client(host, port);
    client.establishConnectionToHost();
    createSyncDir();

    // test
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

void Box::interface( ) {
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

Instruction Box::prepare_instruction(char line[]) {

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

int Box::get_command_id(char *command) {
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