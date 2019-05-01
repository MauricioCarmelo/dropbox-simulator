#include <iostream>
#include <thread>
#include <string>

#define UPLOAD "upload"
#define DOWNLOAD "download"
#define DELETE "delete"
#define LIST_SERVER "list_server"
#define LIST_CLIENT "list_client"
#define GET_SYNC_DIR "get_sync_dir"
#define EXIT "exit"

using namespace std;

void th_func_console_monitor(){

    cout << "Monitorig console began" << endl;
    string command;

    while(command != EXIT){
        cin >> command;

        if(command == UPLOAD) {
            cout << "Upload command" << endl;
        }
        else if(command == DOWNLOAD){
            cout << "Download command" << endl;
        }
        else if(command == DELETE){
            cout << "Delete command" << endl;
        }
        else if(command == LIST_SERVER){
            cout << "List server command" << endl;
        }
        else if(command == LIST_CLIENT){
            cout << "List client command" << endl;
        }
        else if(command == GET_SYNC_DIR){
            cout << "Get sync dir command" << endl;
        }
        else if(command == EXIT);
    }

    cout << "Monitoring console stopped" << endl;
}

int main() {

    cout << "Main started" << endl;

    thread th_console_monitor(th_func_console_monitor);

    cout << "Main continued" << endl;

    th_console_monitor.join();

    return 0;
}