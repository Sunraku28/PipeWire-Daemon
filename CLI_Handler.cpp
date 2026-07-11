#include "AudioConfig.h"
#include "CLI_Handler.h"
#include <string>
#include <iostream>

namespace {
    void print_help() {
        std::cout<<"Audio Router CLI Usage:\n";
        std::cout<< " audiorouter                              Start background routing daemon\n";
        std::cout<< " audiorouter list                         List all configuration audio rules\n";
        std::cout<< " audiorouter set <app> <sink> <volume>    Add or update a routing rule\n";
        std::cout<< " audiorouter remove <app>                 Delete a routing rule\n";
        std::cout<< " audiorouter set-volume <app> <vol>       Update the volume of required application in rules (Here 0.5 will represent 50"<<"% "<< "volume\n"; 
        std::cout<< "Example:\n";
        std::cout<<" audiorouter set spotify speaker 0.5\n";
    }


    void handle_list() {
        json data = load_raw_json();
        std::cout << "\n---------------------------------------------------------- Current Routing Rules -----------------------------------------------------------------------------\n";
        
        if(!data.contains("rules") || data["rules"].empty()) {
            std::cout << "                                          No configuration in " << get_config_path() <<"\n";
        std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            return;
        }

        for(const auto& r : data["rules"]) {
            std::cout << "App Name: [" << r.value("app_name", "UNKNOWN") << "] | Sink: [" << r.value("target_sink", "UNKNOWN") << "] | Volume: [" << r.value("volume", 1.0f) << "]\n"; 
        }
        std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    void handle_set(const std::string& app, const std::string& sink, float vol) {
        json data = load_raw_json();
        
        if(!data.contains("rules")) {
            data["rules"] = json::array();
            return;
        }

        bool updated = false;

        for(auto& r : data["rules"]) {
            if(r.value("app_name", "") == app) {
                r["target_sink"] = sink;
                r["volume"] = vol;
                updated = true;
                break;
            }
        }

        if(!updated) {
            data["rules"].push_back({
                {"app_name", app},
                {"target_sink", sink},
                {"volume", vol}
            });
        }

        save_raw_json(data);
        std::cout << "[SUCCESS] Rule " << (updated ? "updated" : "added") << " for '" << app << "'.\n";
        std::cout << " -> Target Sink: " << sink << " | Volume: " << vol << "\n";
    }

    void handle_remove(const std::string& app) {
        json data = load_raw_json();

        if(!data.contains("rules")) {
            return;
        }
        auto& rules = data["rules"];

        for(auto it = rules.begin(); it!=rules.end();) {
            if((*it).value("app_name", "") == app) {
                it = rules.erase(it);
                save_raw_json(data);
                std::cout << "-> " << app << " routing removed [successfully]\n";
                return;
            }
            else {
                ++it;
            }
        }
        std::cout << "[Warning] No rule found for " << app << "\n";
    }

    void handle_set_vol(const std::string& app, float vol) {
        json data = load_raw_json();
        if(!data.contains("rules")) {
            return;
        }

        auto& rules = data["rules"];

        for(auto it=rules.begin(); it!=rules.end();) {
            if((*it).value("app_name", "") == app) {
                (*it)["volume"] = vol;
                save_raw_json(data);
                std::cout << "Volume " << app << " -> " << vol <<"\n";
                return;
            }
            else {
                ++it;
            }
        }
        std::cout << "[Warning] No rule found for " << app << "\n";

    } 
}


int run_cli(int argc, char* argv[]) {
    std::string command = argv[1];

    if(command == "list") {
        handle_list();
        return 0;
    }
    else if(command == "set") {
        if(argc < 5) {
            std::cerr << "[ERROR] Missing arguments for 'set'.\n";
            std::cerr << "Usage: audiorouter set <app_name> <target_sink> <volume>\n";
            return 1;
        }
        std::string app = argv[2];
        std::string sink = argv[3];
        try {
            float vol = std::stof(argv[4]);
            handle_set(app,sink,vol);
        }
        catch(...) {
            std::cerr << "[ERROR] Invalid volume number provided: '" << argv[4] << "'\n";
            return 1;
        }
        return 0;
    }
    else if(command == "remove") {
        if(argc < 3) {
            std::cerr << "[ERROR] Missing argument for 'remove'. Usage: audiorouter remove <app_name>\n";
            return 1;
        }
        handle_remove(argv[2]);
        return 0;
    }
    else if(command == "set-volume") {
        if(argc < 4) {
            std::cerr << "[ERROR] Missing argument for 'set_volume. Usage: audiorouter set-volume <app_name>\n";
            return 1;
        }
        handle_set_vol(argv[2],std::stof(argv[3]));
        return 0;
    }
    else {
        print_help();
        return 0;
    }
}