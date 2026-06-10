#include <cstdio>
#include <iostream>
#include "SinkNode.h"

std::string get_sink_id() {
    FILE* pipe = popen("wpctl status", "r");
    if(!pipe) return "";

    char buffer[1024];
    std::string sink_id = "";
    std::string line = "";

    while(fgets(buffer, sizeof(buffer), pipe) != nullptr) {

        line = buffer;
        
        if(line.find("Virtual_Speaker") != std::string::npos) {
            size_t first_digit = line.find_first_of("0123456789");

            if (first_digit != std::string::npos) {
                size_t end_of_digit = line.find_first_not_of("0123456789", first_digit);
                sink_id = line.substr(first_digit, end_of_digit - first_digit);
                break;
            }
        }
    }

    pclose(pipe);
    return sink_id;
}