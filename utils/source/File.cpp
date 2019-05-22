#include "../include/File.h"

File::File() = default;

string File::getName()
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
}