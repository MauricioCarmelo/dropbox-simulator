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
        std::cout << "Você digitou o comando " << command_name << " cujo ID eh " << command_id;
        if(strcmp(filename, "") != 0)
            std::cout << " com o arquivo " << path << filename << std::endl;
        else
            std::cout << "\n";
    }
}

void Instruction::upload_file(){
    cout << "Upload function called..." << endl;

    char* filepath = (char*)malloc(sizeof(path) + sizeof(filename));
    strcpy(filepath, path);
    strcat(filepath, filename);

    ifstream file(filepath);

    if(file){
        file.seekg(0, file.end);
        long length = file.tellg();
        file.seekg(0, file.beg);

        char* fileContent = (char*)malloc(length);

        cout << "Reading " << path << filename << endl;
        file.read(fileContent, length);

        if(file){
            cout << "Whole file read successfully, " << length << " bytes read" << endl;
            cout << "File content: " << fileContent << endl;
        }
        else
            cout << "Error: couldn't read whole file" << endl;

        file.close();

        delete[] fileContent;
    }
    else
        cout << "Error: couldn't find file " << path << filename << endl;

    delete[] filepath;
}

void Instruction::download_file(){
    cout << "Download function called..." << endl;

    // fileContent será recebido do servidor (por parâmetro?)
    // então essas linhas não serão necessárias
    char str[] = "esse é um exemplo de conteúdo do arquivo recebido do servidor.";
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

    cout << "Arquivo " << filename << " baixado do servidor" << endl;
}

void Instruction::delete_file(){
    cout << "Delete function called..." << endl;

    char* filepath = (char*)malloc(sizeof(PATH_TO_SYNC_DIR) + sizeof(path) + sizeof(filename));
    strcpy(filepath, PATH_TO_SYNC_DIR);
    strcat(filepath, path);
    strcat(filepath, filename);

    int remove_result = remove(filepath);
    if(remove_result != 0)
        perror("Error deleting file");
    else
        cout << "File " << filename << " deleted succesfully" << endl;

    delete[] filepath;
}

void Instruction::list_server(){
    cout << "List server function called..." << endl;
}

void Instruction::list_client(){
    cout << "List client function called..." << endl;

    DIR* dir;
    struct dirent* ent;
    dir = opendir(PATH_TO_SYNC_DIR);
    if(dir == NULL)
        perror("Error opening sync_dir");
    else{
        while((ent = readdir(dir)) != NULL){
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) // avoid printing "." and ".."
                cout << ent->d_name << endl;
        }
    }
}

void Instruction::get_sync_dir(){
    cout << "Get sync dir function called..." << endl;
}

void Instruction::exit(){
    cout << "Exit function called..." << endl;
}
