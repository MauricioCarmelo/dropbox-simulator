//
// Created by root on 20/04/19.
//

#ifndef DROPBOX_FILEMANAGER_H
#define DROPBOX_FILEMANAGER_H


#include "File.h"
#include "../../utils/include/masterInclude.h"

class FileManager {
private:
    std::vector<File> files;

public:
    FileManager() {};
    ~FileManager() {};
    void openFile(std::string filename, char* buffer);
    bool createDir(char* dirName);
};


#endif //DROPBOX_FILEMANAGER_H
