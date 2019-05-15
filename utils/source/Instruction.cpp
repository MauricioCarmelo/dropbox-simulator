#include "../include/Instruction.h"

void Instruction::prepare(char *line) {
    if(strcmp(line, "\n") == 0){
        cout << "You typed enter only";
    }
    auto cmd_name = strtok(line, " ");
    strcpy(command_name, cmd_name);
    set_command_id();

    switch (command_id)
    {
        case UPLOAD: {
            line = strtok(NULL, "");
            if(line[0] == '/'){
                char *pointer_to_name = strrchr(line, '/');

                strncpy(path, line, pointer_to_name - line + 1);
                strcpy(filename, pointer_to_name + 1);
            }
            else{
                cout << "[Instruction] Error: file parameter from upload command must begin with '/'" << endl;
                command_id = INVALID_COMMAND;
            }
            break;
        }
        case DOWNLOAD:
        case DELETE: {
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
        cout << "[Instruction] Você digitou o comando " << command_name << " cujo ID eh " << command_id;
        if(strcmp(filename, "") != 0)
            cout << " com o arquivo " << path << filename << endl;
        else
            cout << "\n";
    }
}

void Instruction::set_filename(char filename[]){
    strcpy(this->filename, filename);
}

void Instruction::set_path(char path[]) {
    strcpy(this->path, path);
}

void Instruction::reset(){
    command_id = INVALID_COMMAND;
    for(int i = 0; i < 50; i ++){
        filename[i] = '\0';
        command_name[i] = '\0';
        path[i] = '\0';
    }
}

void Instruction::upload_file(Client client){
    cout << "[Instruction] Upload function called..." << endl;

    char* filepath = (char*)malloc(sizeof(path) + sizeof(filename));
    strcpy(filepath, path);
    strcat(filepath, filename);

    ifstream file_read(filepath, ifstream::binary);

    if(file_read){
        file_read.seekg(0, file_read.end);
        long length = file_read.tellg();
        file_read.seekg(0, file_read.beg);

        char* fileContent = (char*)malloc(length);

        cout << "[Instruction] Reading " << filepath << endl;
        file_read.read(fileContent, length);

        if(file_read){
            cout << "[Instruction] Whole file read successfully, " << length << " bytes read" << endl;
            //cout << "File content: " << fileContent << endl;

            client.sendFile(filename, length, fileContent);
        }
        else
            cout << "[Instruction] Error: couldn't read whole file" << endl;

        file_read.close();

        delete[] fileContent;
    }
    else
        cout << "[Instruction] Error: couldn't find file " << filepath << endl;

    delete[] filepath;
}

void Instruction::download_file(Client client){
    cout << "Download function called..." << endl;

    // fileContent será recebido do servidor (por parâmetro?)
    // então essas linhas não serão necessárias
    char str[] = "[Instruction] esse é um exemplo de conteúdo do arquivo recebido do servidor.";
    char* fileContent = (char*)malloc(sizeof(str));
    strcpy(fileContent, str);

    // filepath setado para o cmake-build-debug pois é lá que está o executável => mudar no futuro
    char* filepath = (char*)malloc(sizeof("../cmake-build-debug/") + sizeof(filename));
    strcpy(filepath, "../cmake-build-debug/");
    strcat(filepath, filename);

    ofstream file(filepath);
    file.write(fileContent, strlen(fileContent));
    file.close();

    delete[] fileContent;
    delete[] filepath;

    cout << "[Instruction] Arquivo " << filename << " baixado do servidor" << endl;
}

void Instruction::delete_file(Client client){
    cout << "[Instruction] Delete function called..." << endl;

    client.deleteFile(filename);

}

void Instruction::list_server(Client client){
    cout << "[Instruction] List server function called..." << endl;

    client.list_server();

}

void Instruction::list_client(){
    cout << "[Instruction] List client function called..." << endl;

    DIR* dir;
    struct dirent* ent;
    dir = opendir(PATH_TO_SYNC_DIR);
    if(dir == NULL)
        perror("Error opening sync_dir");
    else{
        while((ent = readdir(dir)) != NULL){
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0){ // avoid printing "." and ".."
                stringstream ss;
                ss << PATH_TO_SYNC_DIR << ent->d_name;
                string filepath_and_name = ss.str();
                struct stat file_status;

                if(stat(filepath_and_name.c_str(), &file_status) == 0){
                    timespec modification_time = file_status.st_mtim;
                    timespec access_time = file_status.st_atim;
                    timespec changed_time = file_status.st_ctim;
                    char* formatted_time = (char*)malloc(32);

                    cout << "File: " << ent->d_name << endl;

                    format_from_timespec_to_string(formatted_time, modification_time);
                    cout << "\tModification time: " << formatted_time << endl;

                    format_from_timespec_to_string(formatted_time, access_time);
                    cout << "\tAccess time: " << formatted_time << endl;

                    format_from_timespec_to_string(formatted_time, changed_time);
                    cout << "\tChange time: " << formatted_time << endl;

                    //cout << ent->d_name << ", " << ctime(&time_without_millisec); // alternative (prints Tue May 14 14:12:08 2019)

                    delete[] formatted_time;
                }
            }
        }
    }
}

void Instruction::get_sync_dir(){
    cout << "[Instruction] Get sync dir function called..." << endl;
}

void Instruction::exit(Client client){
    cout << "[Instruction] Exit function called..." << endl;
    client.sendExitCommand();
}
