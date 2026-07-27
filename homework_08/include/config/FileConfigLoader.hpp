#pragma once

#include <string>

#include "interfaces/IConfigLoader.hpp"

namespace homework_08{

    class FileConfigLoader : public IConfigLoader {
        private:
        Config m_config;
        AmmoParams m_ammo;
        std::string filePath_;

        public:
        FileConfigLoader(const std::string& filepath);

        void load() override;
        Config getConfig() override;
        AmmoParams getAmmoParams() override;
        
        ~FileConfigLoader() {};
    };

}
 