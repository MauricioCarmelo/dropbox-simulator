//
// Created by root on 20/04/19.
//

#ifndef DROPBOX_FILE_H
#define DROPBOX_FILE_H


#include <string>


struct fileDescriptor {
    std::string name;
    // lastUpdatedDate
    int size;
};

class File {
private:
    fileDescriptor descriptor;
public:
    File();
    ~File() {};
    std::string getName();
    int getSize();
    fileDescriptor getDescriptor();
};


#endif //DROPBOX_FILE_H
