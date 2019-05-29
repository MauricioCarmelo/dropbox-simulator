#ifndef DROPBOX_SYSTEMDAO_H
#define DROPBOX_SYSTEMDAO_H

#include "../../utils/include/FileManager.h"
#include "../../utils/include/masterInclude.h"

class SystemDAO {

private:
    FileManager fm;

public:
    SystemDAO();
    ~SystemDAO() {};
    bool createDir(char *dir_path);

};

#endif //DROPBOX_SYSTEMDAO_H
