#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include "json.hpp"

using json = nlohmann::json;

const std::string CONFIG_FILE = "config.json";

struct AudioRules {
    std::string app_name;
    std::string target_sink;
    float volume;
};

inline json load_raw_json() {
    std::ifstream file(CONFIG_FILE);
    if(!file.is_open()) {
        return json{{"rules", json::array()}};
    }
    try {
        return json::parse(file);
    }
    catch(...) {
        return json{{"rules", json::array()}};
    }
}

// Save The JSON

inline void save_raw_json(const json& data) {
    std::ofstream file(CONFIG_FILE);
    if(file.is_open()) {
        file << data.dump(4) << std::endl;
    }
    else {
        std::cerr << "Unable to write " << CONFIG_FILE << "\n";
    }
}

// Load those input AudioRules

inline std::vector<AudioRules> get_rules() {
    std::vector<AudioRules> rules;
    json data = load_raw_json();

    if(data.contains("rules")) {
        for(const auto& r : data["rules"]) {
            AudioRules rule;
            rule.app_name = r.value("app_name", "");
            rule.target_sink = r.value("target_sink", "");
            rule.volume = r.value("volume", 1.0f);

            if(!rule.app_name.empty() && !rule.target_sink.empty()) {
                rules.push_back(rule);
            }
        }
    }

    return rules;
}