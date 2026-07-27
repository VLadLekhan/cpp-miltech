#pragma once

#include "Types.hpp"

namespace homework_08{
    struct DronePosition {
       float x = 0.0f;
       float y = 0.0f;
    };

    class IConfigLoader{
        public:
        virtual void load() = 0;
        virtual Config getConfig() = 0;
        virtual AmmoParams getAmmoParams() = 0;
        virtual ~IConfigLoader() {};
    };
}