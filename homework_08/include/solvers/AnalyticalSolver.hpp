#pragma once

#include "interfaces/IBallisticSolver.hpp"

namespace homework_08 {
class AnalyticalSolver : public IBallisticSolver {
    public:
    AnalyticalSolver();
    DropPoint csolve(
        float xd, float yd, float zd, 
        float targetX, float targetY, 
        float attackSpeed, float accelerationPath, 
        const AmmoParams& ammo) override;
    
    ~AnalyticalSolver() {};
};
}