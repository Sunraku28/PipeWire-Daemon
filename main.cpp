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

struct AppliedState {
    std::string node_id = "";
    std::string sink = "";
    float volume = -1.0f;
};

int main(int argc, char* argv[]) {
    if(argc > 1) {
        return run_cli(argc, argv);
    }
    
    std::cout << "***Booting Dynamic Audio Router Daemon***" << "\n";
    
    std::map<std::string,AppliedState> active_states; // This map tracks App_Name -> Last Routed node ID
    
    while(true) {
        std::vector<AudioRules> rules = get_rules();
        std::map<std::string,std::string> active_streams = get_active_streams();

        for(const auto& stream : active_streams) {
            std::string app_name = stream.first;
            std::string node_id = stream.second;
            
            for(const auto& rule : rules) {
                if(app_name.find(rule.app_name) != std::string::npos) {

                    auto& current = active_states[app_name];

                    if(current.node_id != node_id || current.sink != rule.target_sink) {
                        std::cout << "[Route Updated] " << "App Name: " << app_name << " [on] Node Id: " << node_id << "\n";

                        std::string route_cmd = "pw-metadata " + node_id + " target.object \"" + rule.target_sink + "\"";
                        system(route_cmd.c_str());

                        current.node_id = node_id;
                        current.sink = rule.target_sink;
                    }

                    if(current.volume != rule.volume) {
                        std::cout << "[Volume Updated] " << app_name << " (ID: " << node_id << ") -> " << (rule.volume * 100) << "%\n";
                        std::string vol_cmd = "wpctl set-volume " + node_id + " " + std::to_string(rule.volume);
                        system(vol_cmd.c_str());
                        
                        current.volume = rule.volume;
                    }
                    break;
                }
            }
        }

        for(auto it = active_states.begin(); it != active_states.end();) {
            if(active_streams.find(it->first) == active_streams.end()) {
                it = active_states.erase(it);
            }
            else {
                it++;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}