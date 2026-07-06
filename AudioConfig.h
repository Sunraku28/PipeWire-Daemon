#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

struct AudioRules {
    std::string app_name;
    std::string target_sink;
    float volume;
};

inline std::string get_config_path() {
    const char* home = std::getenv("HOME");
    std::string base_dir = (home != nullptr) ? std::string(home) : ".";
    fs::path config_dir = fs::path(base_dir) / ".config" / "audiorouter";
    
    // Create ~/.config/audiorouter directory automatically if missing
    if (!fs::exists(config_dir)) {
        fs::create_directories(config_dir);
    }
    
    return (config_dir / "config.json").string();
}

inline json load_raw_json() {
    std::string file_path = get_config_path();
    std::ifstream file(file_path);
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
    std::string file_path = get_config_path();
    std::ofstream file(file_path);
    if(file.is_open()) {
        file << data.dump(4) << std::endl;
    }
    else {
        std::cerr << "Unable to write " << file_path << "\n";
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