#include <iostream>
#include <string>
#include <cstdio>


int main() {
    const char* command = "pw-mon";
    FILE* pipe = popen(command, "r");

    if(pipe == nullptr) {
        std::cout << "Unable to monitor the audio" << '\n';
        return -1;
    }

    std::cout << "Successfull Connection to pw-mon" << " " << "Listening to the Audio Input" <<'\n';

    bool is_node = false;
    char buffer[1024];
    std::string current_id = "";

    while(fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string input(buffer);
        size_t id_pos = input.find("object.id =");

        if(id_pos != std::string::npos) {
            current_id = input.substr(id_pos + 4);  
            current_id.erase(0,8);
            is_node = false;    
        }

        if(input.find("PipeWire:Interface:Node") != std::string::npos) {
            is_node = true;
        }

        if(input.find("spotify") != std::string::npos && is_node) {
            std::cout << "Spotify Active" <<'\n';
            std::cout << "ID: " << current_id <<'\n';
            current_id = "";
            is_node = false;
        }
    }

    int exitCode = pclose(pipe);
    std::cout << "pw-mon exited with code " << exitCode << '\n';

    return 0;
}