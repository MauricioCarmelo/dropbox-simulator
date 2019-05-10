#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>

using namespace std;

#define MAX_EVENT_MONITOR 2048
#define FILE_NAME_LENGHT 32
#define MONITOR_SINGLE_EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_LENGHT MAX_EVENT_MONITOR * (MONITOR_SINGLE_EVENT_SIZE + FILE_NAME_LENGHT)

int main() {

    const char* folder = "/home/maicon/CLionProjects/TestInotify/cmake-build-debug/pastateste";
    char buffer[BUFFER_LENGHT];

    int inotify = inotify_init();
    if(inotify < 0){
        cout << "INotify not initialized!" << endl;
        exit(1);
    }

    int watch = inotify_add_watch(inotify, folder, IN_CREATE | IN_MODIFY | IN_DELETE);

    if(watch == -1){
        cout << "Error watching folder " << folder << endl;
        exit(1);
    }

    cout << "Monitor added to folder " << folder << endl;

    while(1){

        int total_read = read(inotify, buffer, BUFFER_LENGHT);
        if(total_read < 0){
            cout << "Read error" << endl;
            exit(1);
        }

        int i = 0;
        while(i < total_read){
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if(event->len){

                if(event->mask & IN_CREATE){
                    if(event->mask & IN_ISDIR)
                        cout << "Directory " << event->name << " was created." << endl;
                    else
                        cout << "File " << event->name << " was created." << endl;
                }
                if(event->mask & IN_MODIFY){
                    if(event->mask & IN_ISDIR)
                        cout << "Directory " << &(event->name) << " was modified." << endl;
                    else
                        cout << "File " << &(event->name) << " was modified." << endl;
                }
                if(event->mask & IN_DELETE){
                    if(event->mask & IN_ISDIR)
                        cout << "Directory " << event->name << " was modified." << endl;
                    else
                        cout << "File " << event->name << " was modified." << endl;
                }
                i += MONITOR_SINGLE_EVENT_SIZE + event->len;
            }
        }
    }

    inotify_rm_watch(inotify, watch);
    close(inotify);

    return 0;
}