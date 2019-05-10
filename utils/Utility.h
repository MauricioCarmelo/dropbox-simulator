#ifndef DROPBOX_UTILITY_H
#define DROPBOX_UTILITY_H

#include <iostream>
#include <string.h>
#include "Instruction.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/poll.h>

#define SYNC_DIR "sync_dir"

#define MAX_EVENT_MONITOR 2048
#define FILE_NAME_LENGHT 32
#define MONITOR_SINGLE_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LENGHT MAX_EVENT_MONITOR * (MONITOR_SINGLE_EVENT_SIZE + FILE_NAME_LENGHT)

void waitForUserCommand();
void inotify_watcher();

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
