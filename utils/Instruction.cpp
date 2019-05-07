#include "Instruction.h"

void Instruction::prepare(char *line) {
    auto cmd_name = strtok(line, " ");
    strcpy(command_name, cmd_name);
    set_command_id();

    switch (command_id)
    {
        case UPLOAD: {
            line = strtok(NULL, "");
            char *pointer_to_name = strrchr(line, '/');

            strncpy(path, line, pointer_to_name - line + 1);
            strcpy(filename, pointer_to_name + 1);
            break;
        }
        case DOWNLOAD: {
            line = strtok(NULL, "");
            strcpy(filename, line);
            strcpy(path, "");
            break;
        }
        case INVALID_COMMAND: {
            break;
        }
        default: {
            strcpy(path, "");
            strcpy(filename, "");
            break;
        }
    }
}

int Instruction::get_command_id() {
    return command_id;
}

void Instruction::set_command_id() {
    if ( strcmp(command_name, "upload") ==  0 )
        command_id = UPLOAD;
    else if ( strcmp(command_name, "download") ==  0 )
        command_id = DOWNLOAD;
    else if ( strcmp(command_name, "delete") ==  0 )
        command_id = DELETE;
    else if ( strcmp(command_name, "list_server") ==  0 )
        command_id = LIST_SERVER;
    else if ( strcmp(command_name, "list_client") ==  0 )
        command_id = LIST_CLIENT;
    else if ( strcmp(command_name, "get_sync_dir") ==  0 )
        command_id = GET_SYNC_DIR;
    else if( strcmp(command_name, "exit") == 0 )
        command_id = EXIT;
    else
        command_id = INVALID_COMMAND;
}

void Instruction::print(){
    if(command_id != INVALID_COMMAND){
        std::cout << "Você digitou o comando " << command_name << " cujo ID eh " << command_id;
        if(strcmp(filename, "") != 0)
            std::cout << " com o arquivo " << path << filename << std::endl;
        else
            std::cout << "\n";
    }
}

void Instruction::upload_file(){
    cout << "Upload function called..." << endl;
}

void Instruction::download_file(){
    cout << "Download function called..." << endl;
}

void Instruction::delete_file(){
    cout << "Delete function called..." << endl;
}

void Instruction::list_server(){
    cout << "List server function called..." << endl;
}

void Instruction::list_client(){
    cout << "List client function called..." << endl;
}

void Instruction::get_sync_dir(){
    cout << "Get sync dir function called..." << endl;
}

void Instruction::exit(){
    cout << "Exit function called..." << endl;
}
