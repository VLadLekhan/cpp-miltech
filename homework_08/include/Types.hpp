#pragma once

namespace homework_08 {
enum class SolverType {
    ANALYTICAL
};

enum class ProviderType {
    JSON
};

enum class LoaderType {
    FILE
};

struct Target {
    float x;
    float y;
};

struct Points2D {
    float x;
    float y;
};

struct DropPoint {
    Points2D fire;
    Points2D inter; 
    bool has_inter_point;
};

struct AmmoParams {   
    float m;
    float d;
    float l;
};

struct Config{
    float zd;
    float attackSpeed;
    float accelerationPath;
};
}