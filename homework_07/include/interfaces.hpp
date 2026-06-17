#pragma once 


struct Target {
    float x;
    float y;
};

struct DropPoint {
    Target fire;
    Target inter; 
    bool has_inter_point;
};

struct AmmoParams {   
    float m;
    float d;
    float l;
};

class IBallisticSolver{
    public:
    virtual DropPoint csolve(
        float xd, float yd, float zd, 
        float targetX, float targetY, 
        float attackSpeed, float accelerationPath, 
        const AmmoParams& ammo) = 0;
        
    virtual ~IBallisticSolver() {}
};

struct Config{
    float zd;
    float attackSpeed;
    float accelerationPath;
};

class IConfigLoader{
    public:
    virtual void load(const char* filepath) = 0;
    virtual Config getConfig() = 0;
    virtual AmmoParams getAmmoParams() = 0;

    virtual ~IConfigLoader() {};
};

class ITargetProvider {
    public:
    virtual int getTargetCount() =0;
    virtual Target getTarget(int idx) =0;

    virtual ~ITargetProvider() {};
};

enum class SolverType   { ANALYTICAL };
enum class ProviderType { JSON };
enum class LoaderType   { FILE };

IBallisticSolver* createSolver (SolverType type);
ITargetProvider* createProvider (ProviderType type, const char* params);
IConfigLoader* createLoader (LoaderType type);    
