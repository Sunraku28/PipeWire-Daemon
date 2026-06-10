#include <iostream>
#include <string>
#include <cstdio>
#include <chrono>
#include <thread>
#include "NodeScanner.h"
// #include "SinkNode.h"

int main() {
    const std::string Target_Sink_ID = "Virtual_Speaker"; // sink id
    std::string last_route = "";

    while(true) {
        std::string spotify_id = get_node_id();
        // std::string spotify_id = "100";

        if(!spotify_id.empty()) {

            std::string command = "pw-metadata " + spotify_id + " target.object " + Target_Sink_ID;
            system(command.c_str());

            last_route = spotify_id;
        }

        if(spotify_id.empty()) {
            last_route = "";
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));

    }

    return 0;
}
