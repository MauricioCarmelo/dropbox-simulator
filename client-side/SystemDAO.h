//
// Created by root on 20/04/19.
//

#ifndef DROPBOX_SYSTEMDAO_H
#define DROPBOX_SYSTEMDAO_H


#include "FileManager.h"
#include <string>

class SystemDAO {

private:
    FileManager fm;

public:
    bool createDir(std::string path);

};


#endif //DROPBOX_SYSTEMDAO_H
