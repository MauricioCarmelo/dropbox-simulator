//
// Created by vagrant on 5/4/19.
//

#ifndef DROPBOX_UTILITY_H
#define DROPBOX_UTILITY_H

#include <iostream>
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

/*struct Instruction {
    char command_name[50];
    int command_id;
    char path[50];
    char filename[50];
};*/

void waitForUserCommand();
Instruction prepare_instruction(char line[]);
int get_command_id(char *command);
void print_instruction(Instruction inst);

// Exemplo:
//  unsigned char* buffer;
//  struct_to_unsigned_char_ptr(buffer, s); // s é a variável da struct T
template <typename T> void struct_to_unsigned_char_ptr(unsigned char*& array, T& strct){
    memcpy(array, (const unsigned char*)&strct, sizeof(T));
}

// Exemplo:
//  T s; // T é o tipo da struct, s é a variável
//  unsigned_char_ptr_to_struct(buffer, s);
template <typename T> void unsigned_char_ptr_to_struct(unsigned char *&array, T &strct){
    memcpy(&strct, (T*)array, sizeof(T));
}

// Exemplo:
//  unsigned char* buffer;
//  alloc_unsigned_char_ptr_to_type_T<T>(buffer); // T é o tipo da struct
template <typename T> void alloc_unsigned_char_ptr_to_type_T(unsigned char*& array){
    array = (unsigned char*)malloc(sizeof(T));
}

#endif //DROPBOX_UTILITY_H
