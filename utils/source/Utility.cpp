#include "../include/Utility.h"

using namespace std;

void format_from_timespec_to_string(char *formatted_time, timespec ts){
    time_t time_withou_millisec = ts.tv_sec;
    tm* time = localtime(&time_withou_millisec);
    strftime(formatted_time, 32, "%d/%m/%y %H:%M:%S", time);
}