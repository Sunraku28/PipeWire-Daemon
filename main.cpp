#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "NodeScanner.h"
#include "CLI_Handler.h"
#include "AudioConfig.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[]) {
    if(argc > 1) {
        return run_cli(argc, argv);
    }
    
    std::cout << "***Booting Dynamic Audio Router Daemon***" << "\n";
    
    std::vector<AudioRules> rules = get_rules();
    std::cout << "Loaded Rules : " << rules.size() << " application routing rules.\n\n";
    
    std::map<std::string,std::string> routed_sessions; // This map tracks App_Name -> Last Routed node ID
    
    while(true) {
        std::map<std::string,std::string> active_streams = get_active_streams();

        for(const auto& stream : active_streams) {
            std::string app_name = stream.first;
            std::string node_id = stream.second;
            
            for(const auto& rule : rules) {
                if(app_name.find(rule.app_name) != std::string::npos) {

                    if(routed_sessions[app_name] != node_id) {
                        std::cout << "[New Session Detected] " << "App Name: " << app_name << " [on] Node Id: " << node_id << "\n";

                        std::string route_cmd = "pw-metadata " + node_id + " target.object " + rule.target_sink;
                        system(route_cmd.c_str());

                        std::string vol_cmd = "wpctl set-volume " + node_id + " " + std::to_string(rule.volume);
                        system(vol_cmd.c_str());

                        routed_sessions[app_name] = node_id; // Update the current routed sessions
                    }
                    break;
                }
            }
        }

        for(auto it = routed_sessions.begin(); it != routed_sessions.end();) {
            if(active_streams.find(it->first) == active_streams.end()) {
                it = routed_sessions.erase(it);
            }
            else {
                it++;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}