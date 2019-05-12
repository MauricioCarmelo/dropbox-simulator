//
// Created by root on 20/04/19.
//

#ifndef DROPBOX_FILE_H
#define DROPBOX_FILE_H


#include "../../utils/include/masterInclude.h"


struct fileDescriptor {
    std::string name;
    // TODO lastUpdatedDate parameter
    //TODO file extension parameter
    int size;
};

class File {
private:
    fileDescriptor descriptor;
public:
    File();
    ~File() = default;;
    std::string getName();
    int getSize();
    fileDescriptor getDescriptor();
};


#endif //DROPBOX_FILE_H
