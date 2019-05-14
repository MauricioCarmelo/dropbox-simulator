#include "../include/Box.h"

bool exit_command_typed = false;
Instruction Box::instruction;

Box::Box()
{
}

char* Box::get_username() {
    return username;
}

void Box::set_username(char *name) {
    bzero(username, USERNAME_SIZE);
    strcpy(username, name);
}

void Box::setUserFolder(char *dir) {
    bzero(userFolderPath, FOLDER_SIZE);
    strcpy(userFolderPath, dir);
}

int Box::open(char *host, int port) {

    //thread th_console(th_func_monitor_console, c1);
    //thread th_inotify(th_func_inotify);

    //client = Client(host, port);
    //client.establishConnectionToHost();
    createSyncDir();
    srand(time(0));

    // test
    //char frase[] = "frase1";
    //client.send(frase, 7);
    //int username = rand()%10;
    int device = rand()%10+1;

    connection_t con;
    con.packetType = CONN;
    con.socketType = T1;
    strcpy(con.username, username);
    con.device = device;

    // open connection
    c1 = Client(host, port);
    c1.establishConnectionToHost();

    if (c1.establishConnectionType(con) == -1 ){
        std::cout << "[Box] ABORTAR, nao foi possivel se conectar" << std::endl;
        return 0;
    }

    /*
    // open connection 2
    c2 = Client(host, port);
    c2.establishConnectionToHost();
    connection_t con2;
    con2.type = T2;
    con2.username = username;
    c2.establishConnectionType(con2);
     */

    thread th_console(th_func_monitor_console, c1);
    thread th_inotify(th_func_inotify, userFolderPath);

    /*while(!exit_command_typed) {
        c1.establishConnectionType(con1);
        c2.establishConnectionType(con2);
    }*/

    th_console.join();
    th_inotify.join();

    std::cout << "TERMINOU" << std::endl;
}

bool Box::createSyncDir() {
    char dir_path[200];

    // convert string to array of char

    if ( sda.createDir(userFolderPath) )
        return true;
    return false;
}

void* Box::th_func_monitor_console(Client client){
    cout << "[Box] Monitor console thread" << endl;

    char line[100];
    while(instruction.get_command_id() != EXIT)
    {
        scanf("%[^\n]", line);
        getchar();

        instruction.prepare(line);

        switch(instruction.get_command_id()){
            case UPLOAD:
                instruction.upload_file(client);
                break;

            case DOWNLOAD:
                instruction.download_file(client);
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
                instruction.exit(client);
                break;

            case INVALID_COMMAND:
                cout << " [Box] Invalid command!" << endl;
                break;

            default: break;
        }
    }

    exit_command_typed = true;
    cout << " [Box] Thread monitoring console finished properly" << endl;
}

void* Box::th_func_inotify(char *folder){
    cout << "[Box] Inotify thread" << endl;

    char buffer[BUFFER_LENGHT];

    int inotify_descriptor = inotify_init();
    if(inotify_descriptor < 0){
        cout << " [Box] Error initializing inotify" << endl;
        //return;
        exit(-1);
    }
    cout << "[Box] Inotify initialized" << endl;


    int watcher_descriptor = inotify_add_watch(inotify_descriptor, folder, IN_CREATE | IN_MODIFY | IN_DELETE);
    if(watcher_descriptor == -1){
        cout << "[Box] Error initiazing inotify watcher" << endl;
        //return;
        exit(-1);
    }
    cout << "[Box] Inotify watcher added to folder " << folder << endl;

    // será usado para criar um timer
    pollfd poll_descriptor;
    poll_descriptor.fd = inotify_descriptor;
    poll_descriptor.events = POLLIN;
    poll_descriptor.revents = 0;

    while(!exit_command_typed){

        // timer de 100ms criado
        int poll_result = poll(&poll_descriptor, 1, 100);

        if(poll_result < 0){
            cout << "[Box] Inotify couldn't monitor changes on folder " << SYNC_DIR;
            //return;
            exit(-1);
        }
        else if(poll_result == 0); // nada para ser lido
        else {
            // read() é bloqueante, por isso é chamado apenas quando há algo para ler (quando poll_result > 0)
            int total_read = read(inotify_descriptor, buffer, BUFFER_LENGHT);
            if(total_read < 0){
                cout << "[Box] Error reading inotify" << endl;
                //return;
                exit(-1);
            }

            int i = 0;
            // ler todos os eventos do buffer
            while(i < total_read){
                struct inotify_event* event = (struct inotify_event*)&buffer[i];
                if(event->len){

                    if(event->mask & IN_CREATE){
                        cout << "[Box] Directory or folder " << event->name << " was created" << endl;
                    }
                    if(event->mask & IN_MODIFY){
                        cout << "[Box] Directory or folder " << event->name << " was modified" << endl;
                    }
                    if(event->mask & IN_DELETE){
                        cout << "[Box] Directory or folder " << event->name << " was deleted" << endl;
                    }

                    i += MONITOR_SINGLE_EVENT_SIZE + event->len;
                }
            }
        }
    }

    inotify_rm_watch(inotify_descriptor, watcher_descriptor);
    close(inotify_descriptor);
    cout << "[Box] Inotify thread finished properly" << endl;
}
