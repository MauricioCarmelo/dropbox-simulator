//
// Created by root on 20/04/19.
//

#include <sys/stat.h>
#include <iostream>
#include "FileManager.h"

void FileManager::openFile(std::string filename, char *buffer) {

}

bool FileManager::createDir(char* dirName) {

    if( mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 ) {
        std::cout << "Folder " << dirName << " created" << std::endl;
        return true;
    }
    std::cout << "Folder " << dirName << " already exists" << std::endl;
    return false;
}