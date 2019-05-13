#include "../include/Instruction.h"

void Instruction::prepare(char *line) {
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
        cout << "[Instruction] Você digitou o comando " << command_name << "[Instruction] cujo ID eh " << command_id;
        if(strcmp(filename, "") != 0)
            cout << " com o arquivo " << path << filename << endl;
        else
            cout << "\n";
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

        cout << "[Instruction] Reading " << path << filename << endl;
        file_read.read(fileContent, length);

        if(file_read){
            cout << "[Instruction] Whole file read successfully, " << length << " bytes read" << endl;
            cout << "File content: " << fileContent << endl;

            file_t file;
            //file.name = (char*)malloc(sizeof(filename));
            strcpy(file.name, filename);
            file.content = (char*)malloc(length);
            strcpy(file.content, fileContent);

            unsigned char* buffer;
            alloc_unsigned_char_ptr_to_type_T<file_t>(buffer);
            struct_to_unsigned_char_ptr(buffer, file);

            cout << strlen(filename) + length << endl;

            /*ofstream gambiarra("gambiarra.txt");
            gambiarra.write((char*)&file, strlen(filename) + length + 1);
            gambiarra.close();*/

            client.send(filename, length, fileContent);

            /*file_t newfile; newfile.name = (char*)malloc(sizeof(filename));newfile.content = (char*)malloc(sizeof(fileContent));
            unsigned_char_ptr_to_struct(buffer, newfile);
            cout << file.name << ", " << file.content << endl;
            cout << newfile.name << ", " << newfile.content << endl;*/
        }
        else
            cout << "[Instruction] Error: couldn't read whole file" << endl;

        file_read.close();

        delete[] fileContent;
    }
    else
        cout << "[Instruction] Error: couldn't find file " << path << filename << endl;

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

void Instruction::delete_file(){
    cout << "[Instruction] Delete function called..." << endl;

    char* filepath = (char*)malloc(sizeof(PATH_TO_SYNC_DIR) + sizeof(path) + sizeof(filename));
    strcpy(filepath, PATH_TO_SYNC_DIR);
    strcat(filepath, path);
    strcat(filepath, filename);

    int remove_result = remove(filepath);
    if(remove_result != 0)
        perror("Error deleting file");
    else
        cout << "[Instruction] File " << filename << "deleted succesfully" << endl;

    delete[] filepath;
}

void Instruction::list_server(){
    cout << "[Instruction] List server function called..." << endl;
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
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) // avoid printing "." and ".."
                cout << ent->d_name << endl;
        }
    }
}

void Instruction::get_sync_dir(){
    cout << "[Instruction] Get sync dir function called..." << endl;
}

void Instruction::exit(){
    cout << "[Instruction] Exit function called..." << endl;
}
