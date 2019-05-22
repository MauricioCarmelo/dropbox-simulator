#ifndef DROPBOX_FILE_H
#define DROPBOX_FILE_H

#include "masterInclude.h"

struct fileDescriptor {
    std::string name;
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
