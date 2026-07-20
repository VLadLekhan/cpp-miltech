#define _USE_MATH_DEFINES
//#include <cstddef>
#include <cmath>
#include "interfaces.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class JsonTargetProvider : public ITargetProvider{
private:
    static const int MAX_TARGET = 50;
    Target targets [MAX_TARGET];
    int actualCountTarget = 0;

    void loadFromJson(const char* filepath){

        std::ifstream file(filepath);
        if (!file.is_open()){
            std::cerr << "ERROR: JSON File" << filepath <<"do not open!" << std::endl;
            return;
        }
        
        json root_json;
        file >> root_json;
        file.close();

        for (const auto& element : root_json) {
            if (actualCountTarget >= MAX_TARGET) {
                std::cerr << "ERROR: Too much targets!" << std::endl;
                break;
            }

            targets[actualCountTarget].x = element["x"].get<float>();
            targets[actualCountTarget].y = element["y"].get<float>();

            actualCountTarget++;
        }

        std::cout << "[INFO]: JsonTargetProvider were downloaded\n"
        << actualCountTarget << "times " << std::endl;
    }

public:
    JsonTargetProvider(const char* filepath) {
        loadFromJson(filepath);
    }

    int getTargetCount() override {
        return actualCountTarget;
    }

    Target getTarget(int idx) override { 
        return targets[idx];
    }
};

class FileConfigLoader : public IConfigLoader {
private:
    Config configuration_;
    AmmoParams ammoParams_;

public:
    FileConfigLoader() {}
 
    void load(const char* filepath) override {

        std::ifstream file(filepath);
        if (!file.is_open()){
            std::cerr << "ERROR: " << filepath << " don`t open!" << std::endl;
            return;
        }

        json config_json;
        file >> config_json;
        file.close();

        configuration_.zd = config_json["zd"].get<float>();
        configuration_.attackSpeed = config_json["attack_speed"].get<float>();
        configuration_.accelerationPath = config_json["accelerationPath"].get<float>();

        ammoParams_.m = config_json["ammo"]["m"].get<float>();
        ammoParams_.d = config_json["ammo"]["d"].get<float>();
        ammoParams_.l = config_json["ammo"]["l"].get<float>();

        std::cout << "[INFO]: FileConfigLoader were downloaded!" << std::endl;
    }

    Config getConfig() override {
        return configuration_;
    }

    AmmoParams getAmmoParams() override {
        return ammoParams_;
    }
};

class AnalyticalSolver : public IBallisticSolver {
private:
    float g_ = 9.81f;
    float pi_ = M_PI;

public:
    DropPoint csolve (
        float xd, float yd, float zd, 
        float targetX, float targetY, 
        float attackSpeed, float accelerationPath, 
        const AmmoParams& ammo) override {

            float m = ammo.m;
            float d = ammo.d;
            float l = ammo.l;

            float a = (d * g_ * m) - (2.0f * d * d * l * attackSpeed);
            float b = (-3.0f * g_ * m * m) + (3.0f * d * l * m * attackSpeed);
            float c = 6.0f * m * m * zd;

            float p, q;

            p = -(b * b) / (3.0f * a * a);
            q = (2.0f * b * b * b) / (27.0f * a * a * a) + (c / a);

            float arg_arccos = ((3.0f * q) / (2.0f * p)) * std::sqrt(-3.0f / p);

            if (arg_arccos < -1.0f || arg_arccos > 1.0f) {
                std::cerr << "CRITICAL ERROR: Physics model failure." << "Argument for arccos(" << arg_arccos << ") is out of domain[-1, 1]."
                        << std::endl;
                throw std::runtime_error("Physics model failure: Arccos domain error");
            }

            float phi = std::acos(arg_arccos);

            float t = (2.0f * std::sqrt(-p / 3.0f)) * std::cos((phi + 4.0f * pi_) / 3.0f) - b / (3.0f * a);

            float h =
                (attackSpeed * t) - (t * t * d * attackSpeed) / (2.0f * m) +
                (std::pow(t, 3) * (6.0f * d * g_ * l * m - 6.0f * d * d * (l * l - 1.0f) * attackSpeed)) / (36.0f * m * m) +
                (std::pow(t, 4) *
                (-6.0f * d * d * g_ * l * (1.0f + l * l + std::pow(l, 4)) * m + 3.0f * std::pow(d, 3) * l * l * (1.0f + l * l) * attackSpeed +
                6.0f * std::pow(d, 3) * std::pow(l, 4) * (1.0f + l * l) * attackSpeed)) /
                (36.0f * std::pow(1.0f + l * l, 2) * std::pow(m, 3)) +
                (std::pow(t, 5) * (3.0f * std::pow(d, 3) * g_ * std::pow(l, 3) * m - 3.0f * std::pow(d, 4) * l * l * (1.0f + l * l) * attackSpeed)) /
                (36.0f * (1.0f + l * l) * std::pow(m, 4));

            float D = std::sqrt(std::pow(targetX - xd, 2) + std::pow(targetY - yd, 2));

            if (D < 0.001f) {
                throw std::runtime_error("Error: Target distance is zero. Cannot compute approach vector.");
            }

            DropPoint result;
            float ratio = (D - h) / D;

            if (h + accelerationPath > D) {
                result.inter = {targetX - (targetX - xd) * (float)((h + accelerationPath) / D),
                                targetY - (targetY - yd) * (float)((h + accelerationPath) / D)};
                result.has_inter_point = true;
            }
            else {
                result.has_inter_point = false;
            }

            result.fire = {xd + (targetX - xd) * ratio, yd + (targetY - yd) * ratio};

            return result;
            }

        };

IBallisticSolver* createSolver (SolverType type) {
    switch (type) {
        case SolverType :: ANALYTICAL :
        return new AnalyticalSolver();

        default:
        return nullptr;
    }
}

ITargetProvider* createProvider (ProviderType type, const char* params) {
    switch(type) {
        case ProviderType :: JSON :
        return new JsonTargetProvider(params);

         default:
        return nullptr;
    }
}

IConfigLoader* createLoader (LoaderType type) {
    switch (type) {
        case LoaderType::FILE:
            return new FileConfigLoader();

        default:
            return nullptr;
    }
}
