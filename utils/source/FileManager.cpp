#include <sys/stat.h>
#include <iostream>
#include "../include/FileManager.h"

void FileManager::openFile(std::string filename, char *buffer) {

}

bool FileManager::createDir(char* dirName) {

    if( mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 ) {
        cout << "[FileManager] Folder " << dirName << " created" << endl;
        return true;
    }
    cout << " [FileManager] Folder " << dirName << " already exists" << endl;
    return false;
}