#include <iostream>
#include <string>
#include <cstdio>

bool is_sink_currently_active(const std::string& sink) {
    FILE* file = popen("wpctl status", "r");
    if(!file) {
        return false;
    }

    char buffer[256];
    std::string wpctl_output = "";

    while(fgets(buffer, sizeof(buffer), file) != nullptr) {
        wpctl_output += buffer;
    } 
    pclose(file);

    return wpctl_output.find(sink) != std::string::npos;
}