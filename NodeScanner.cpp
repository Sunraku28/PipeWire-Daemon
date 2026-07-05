#include <string>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <map>
#include "NodeScanner.h"

std::map<std::string,std::string> get_active_streams() {
    std::map<std::string,std::string> streams;

    FILE* pipe = popen("wpctl status" ,"r");
    if (!pipe) return streams;

    char buffer[1024];
    std::string line;

    bool in_streams = false;

    while(fgets(buffer, sizeof(buffer), pipe) != nullptr) {

        line = buffer;

        if (line.find("Streams:") != std::string::npos) {
            in_streams = true;
        }
        
        if (line.find("Video:") != std::string::npos) {
            in_streams = false;
        }

        if(in_streams) {
            size_t non_space = line.find_first_not_of(" \t");
            if(non_space != std::string::npos) {
                std::string trimmed_line = line.substr(non_space);
                
                if(isdigit(trimmed_line[0])) {
                    size_t dot_pos = trimmed_line.find(". ");

                    if(dot_pos != std::string::npos) {
                        std::string id = trimmed_line.substr(0,dot_pos);
                        std::string name = trimmed_line.substr(dot_pos+2);

                        size_t last_char = name.find_last_not_of(" \r\n");
                        if(last_char != std::string::npos) {
                            name = name.substr(0,last_char+1);
                        }

                        if(name.find("input_") == std::string::npos &&
                           name.find("output_") == std::string::npos &&
                           name.find("monitor_") == std::string::npos) {

                            std::transform(name.begin(),name.end(),name.begin(), ::tolower);
                            streams[name] = id; // ID saved in the map here
                        }
                    }
                }
            }
        }
    }

    pclose(pipe);
    return streams;

}

int main() {
    std::map<std::string,std::string> active_nodes = get_active_streams();

    if(active_nodes.empty()) {
        std::cout << "No active Nodes" << "\n";
    }
    else {
        std::cout << active_nodes.size() << "\n";
        for(const auto& pair : active_nodes) {
            std::cout << "Name " << pair.first << " " << "ID " << pair.second << "\n";
        }
    }

    return 0;
}

