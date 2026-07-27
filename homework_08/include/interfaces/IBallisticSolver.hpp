#pragma once

#include "Types.hpp"

namespace homework_08{

class IBallisticSolver{
    public:
    virtual DropPoint csolve(
        float xd, float yd, float zd, 
        float targetX, float targetY, 
        float attackSpeed, float accelerationPath, 
        const AmmoParams& ammo) = 0;
        
    virtual ~IBallisticSolver() {}
};
}