//
// Created by root on 20/04/19.
//

#include "File.h"

std::string File::getName()
{
    return descriptor.name;
}

int File::getSize()
{
    return descriptor.size;
}

fileDescriptor File::getDescriptor()
{
    return descriptor;
};