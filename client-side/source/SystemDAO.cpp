//
// Created by root on 20/04/19.
//

#include "../include/SystemDAO.h"

SystemDAO::SystemDAO() {
    fm = FileManager();
}

bool SystemDAO::createDir(char* dir_path)
{
    fm.createDir(dir_path);
}