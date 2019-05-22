#include "../include/SystemDAO.h"

SystemDAO::SystemDAO() {
    fm = FileManager();
}

bool SystemDAO::createDir(char* dir_path)
{
    fm.createDir(dir_path);
}