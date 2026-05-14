#pragma once

#include <string>

namespace ballistics {

    struct Position{
        float x;
        float y;
    };

    struct AmmoParams {
        float m;
        float d;
        float l;
    };

    AmmoParams get_ammo_params (const std::string& ammo_name);

    struct Calculation {
        Position fire;
        Position inter;
    };

    Calculation calculate_drop_point(
        float xd,float yd, float zd,
        float targetX, float targetY,
        float attackSpeed, float accelerationPath,
        const AmmoParams& ammo_name
    );


}