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

struct Instruction {
    char command_name[50];
    int command_id;
    char path[50];
    char filename[50];
};

void waitForUserCommand();
Instruction prepare_instruction(char line[]);
int get_command_id(char *command);
void print_instruction(Instruction inst);

/* Exemplo:
 *   char* buffer;
 *   struct_to_char_ptr(buffer, s); // s é a variável da struct
 */
template <typename T> void struct_to_char_ptr(char*& array, T& strct){
    array = (char*)&strct;
}

/* Exemplo:
 *   T* s; // T é a struct
 *   char_ptr_to_struct_ptr(buffer, s); // s é a variável da struct T
 */
template <typename T> void char_ptr_to_struct_ptr(char*& array, T*& strct){
    strct = (T*)array;
}


#endif //DROPBOX_UTILITY_H
