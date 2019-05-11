#include "Utility.h"

using namespace std;

//bool exit_command_typed = false;

/*void waitForUserCommand() {
    char line[100];
    Instruction instruction;

    while(instruction.get_command_id() != EXIT)
    {
        scanf("%[^\n]", line);
        getchar();

        instruction.prepare(line);

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

    exit_command_typed = true;
    cout << "Thread monitoring console finished properly" << endl;
}

void inotify_watcher(){
    const char* folder_path = SYNC_DIR;
    char buffer[BUFFER_LENGHT];

    int inotify_descriptor = inotify_init();
    if(inotify_descriptor < 0){
        cout << "Error initializing inotify" << endl;
        return;
    }
    cout << "Inotify initialized" << endl;

    int watcher_descriptor = inotify_add_watch(inotify_descriptor, folder_path, IN_CREATE | IN_MODIFY | IN_DELETE);
    if(watcher_descriptor == -1){
        cout << "Error initiazing inotify watcher" << endl;
        return;
    }
    cout << "Inotify watcher added to folder " << folder_path << endl;

    // será usado para criar um timer
    pollfd poll_descriptor;
    poll_descriptor.fd = inotify_descriptor;
    poll_descriptor.events = POLLIN;
    poll_descriptor.revents = 0;

    while(!exit_command_typed){

        // timer de 100ms criado
        int poll_result = poll(&poll_descriptor, 1, 100);

        if(poll_result < 0){
            cout << "Inotify couldn't monitor changes on folder " << SYNC_DIR;
            return;
        }
        else if(poll_result == 0); // nada para ser lido
        else {
            // read() é bloqueante, por isso é chamado apenas quando há algo para ler (quando poll_result > 0)
            int total_read = read(inotify_descriptor, buffer, BUFFER_LENGHT);
            if(total_read < 0){
                cout << "Error reading inotify" << endl;
                return;
            }

            int i = 0;
            // ler todos os eventos do buffer
            while(i < total_read){
                struct inotify_event* event = (struct inotify_event*)&buffer[i];
                if(event->len){

                    if(event->mask & IN_CREATE){
                        cout << "Directory or folder " << event->name << " was created" << endl;
                    }
                    if(event->mask & IN_MODIFY){
                        cout << "Directory or folder " << event->name << " was modified" << endl;
                    }
                    if(event->mask & IN_DELETE){
                        cout << "Directory or folder " << event->name << " was deleted" << endl;
                    }

                    i += MONITOR_SINGLE_EVENT_SIZE + event->len;
                }
            }
        }
    }

    inotify_rm_watch(inotify_descriptor, watcher_descriptor);
    close(inotify_descriptor);
    cout << "Inotify thread finished properly" << endl;
}*/