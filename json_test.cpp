#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include "json.hpp"

using json = nlohmann::json;

struct AudioRule {
    std::string app_name;
    std::string target_sink;
    float volume;
};

int main() {
    std::vector<AudioRule> active_rules;

    std::ifstream file("config.json");
    if(!file.is_open()) {
        std::cout << "Error";
        return 1;
    }

    json data = json::parse(file);

    for(const auto &rule : data["rules"]) {
        AudioRule new_rule;
        new_rule.app_name = rule["app_name"];
        new_rule.target_sink = rule["target_sink"];
        new_rule.volume = rule["volume"];

        active_rules.push_back(new_rule);
    }

    std::cout << "\n--- Successfully Loaded " << active_rules.size() << " Routing Rules ---\n";
    
    for (const auto& rule : active_rules) {
        std::cout << "-> APP: " << rule.app_name 
                  << "\n   SINK: " << rule.target_sink 
                  << "\n   VOL:  " << rule.volume << "\n\n";
    }

    return 0;
}