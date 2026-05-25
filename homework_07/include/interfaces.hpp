#pragma once 
#include <string>

struct Target {
    float x =0.0;
    float y =0.0;
    float z =0.0;
    float speed =0.0;
    std::string id = "";
};

struct DropPoint {
    float x =0.0;
    float y =0.0;
};

class ITargetProvider{
public: 
    virtual int getTargetCount() = 0;
    virtual Target getTarget(int index) = 0;

    virtual ~ITargetProvider();
};

class IBallisticSolver{
    virtual DropPoint calculateDropPoint (Target target, float drone_speed, float drone_alt) = 0;

    virtual ~IBallisticSolver();
};
