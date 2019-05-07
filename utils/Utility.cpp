#include "Utility.h"

using namespace std;

void waitForUserCommand() {
    char line[100];
    Instruction instruction;

    while(instruction.get_command_id() != EXIT)
    {
        scanf("%[^\n]", line);
        getchar();

        instruction.prepare(line);
        instruction.print();

        switch(instruction.get_command_id()){
            case UPLOAD:
                instruction.upload_file();
                break;

            case DOWNLOAD:
                instruction.download_file();
                break;

            case DELETE:
                instruction.delete_file();
                break;

            case LIST_SERVER:
                instruction.list_server();
                break;

            case LIST_CLIENT:
                instruction.list_client();
                break;

            case GET_SYNC_DIR:
                instruction.get_sync_dir();
                break;

            case EXIT:
                instruction.exit();
                break;

            case INVALID_COMMAND:
                std::cout << "Invalid command!" << std::endl;
                break;

            default: break;
        }
    }
}

