#include <string>
#include <cstdio>
#include <iostream>
#include "NodeScanner.h"

std::string get_node_id() {
    FILE* pipe = popen("wpctl status" ,"r");
    if (!pipe) return "";

    char buffer[1024];
    std::string line = "";
    std::string node_id = "";

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
            if(line.find("spotify") != std::string::npos || line.find("Spotify") != std::string::npos) {

                size_t first_digit = line.find_first_of("0123456789");

                if (first_digit != std::string::npos) {
                    size_t end_of_digit = line.find_first_not_of("0123456789", first_digit);
                    node_id = line.substr(first_digit, end_of_digit - first_digit);
                    break;
                }
            }
        }
    }

    pclose(pipe);
    // std::cout << node_id << "\n";
    return node_id;

}

// int main() {
//     get_node_id();
// }

