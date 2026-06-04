#include "providers/JsonTargetProvider.hpp"
#include <fstream>
#include <iostream>

#include "Types.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace homework_08 {
    JsonTargetProvider::JsonTargetProvider(const std::string& file_path) 
    : json_path(file_path)
    {
        std::ifstream file(file_path);
    
        if(!file.is_open()) {
          std::cerr << "File: " << file_path << " not open!" << std::endl;
        }

        json data;
        file >> data;

        for (const auto& item : data) {
            Target t;
            t.x = item["x"].get<float>();
            t.y = item["y"].get<float>();

            json_targets.push_back(t);
        }
    }

    int JsonTargetProvider::getTargetsCount() {
        return static_cast<int>(json_targets.size());
    }
    
    Target JsonTargetProvider::getTargetPosition(int idx) {
        return json_targets[idx];
    }

}