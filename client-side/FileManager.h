//
// Created by root on 20/04/19.
//

#ifndef DROPBOX_FILEMANAGER_H
#define DROPBOX_FILEMANAGER_H


#include <vector>

class FileManager {
private:
    std::vector<Files> files;

public:
    FileManager();
    ~FileManager();
    void openFile(std::string filename, char* buffer);


};


#endif //DROPBOX_FILEMANAGER_H
