#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

#include "config/FileConfigLoader.hpp"

using json = nlohmann::json;

namespace homework_08 {
    FileConfigLoader::FileConfigLoader(const std::string& filepath)
    : filePath_(filepath){}

    void FileConfigLoader::load() {
        std::ifstream file (filePath_);

        if(!file.is_open()) {
            std::cerr << "File: " << filePath_ << " is not open!" << std::endl;
        }

        json data;
        file >> data;
        
        m_config.zd = data.at("zd").get<float>();
        m_config.attackSpeed = data.at("attack_speed").get<float>();
        m_config.accelerationPath = data.at("accelerationPath").get<float>();

        auto ammoJson = data.at("ammo");
        m_ammo.m = ammoJson.at("m").get<float>();
        m_ammo.d = ammoJson.at("d").get<float>();
        m_ammo.l = ammoJson.at("l").get<float>();
    }

    Config FileConfigLoader::getConfig() {
        return m_config;
    }

    AmmoParams FileConfigLoader::getAmmoParams() {
        return m_ammo;
    }
}