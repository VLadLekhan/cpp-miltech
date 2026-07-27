#define _USE_MATH_DEFINES
#include "solvers/AnalyticalSolver.hpp"
#include <cmath>
#include <iostream>

namespace homework_08 {
    AnalyticalSolver::AnalyticalSolver() {}
    DropPoint AnalyticalSolver::csolve(float xd, float yd, float zd, 
        float targetX, float targetY, 
        float attackSpeed, float accelerationPath, 
        const AmmoParams& ammo) {
            std::cout << "[DEBUG SOLVER] Позиція дрона Zd: " << zd << std::endl;
            std::cout << "[DEBUG SOLVER] Швидкість снаряда: " << attackSpeed << std::endl;
            std::cout << "[DEBUG SOLVER] Маса снаряда m: " << ammo.m << std::endl;
            std::cout << "[DEBUG SOLVER] Ціль X: " << xd << ", Y: " << yd << std::endl;

            float g = 9.81f;
            float pi = M_PI;

            float m = ammo.m;
            float d = ammo.d;
            float l = ammo.l;

            float a = (d * g * m) - (2.0f * d * d * l * attackSpeed);
            float b = (-3.0f * g * m * m) + (3.0f * d * l * m * attackSpeed);
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

            float t = (2.0f * std::sqrt(-p / 3.0f)) * std::cos((phi + 4.0f * pi) / 3.0f) - b / (3.0f * a);

            float h =
                (attackSpeed * t) - (t * t * d * attackSpeed) / (2.0f * m) +
                (std::pow(t, 3) * (6.0f * d * g * l * m - 6.0f * d * d * (l * l - 1.0f) * attackSpeed)) / (36.0f * m * m) +
                (std::pow(t, 4) *
                (-6.0f * d * d * g * l * (1.0f + l * l + std::pow(l, 4)) * m + 3.0f * std::pow(d, 3) * l * l * (1.0f + l * l) * attackSpeed +
                6.0f * std::pow(d, 3) * std::pow(l, 4) * (1.0f + l * l) * attackSpeed)) /
                (36.0f * std::pow(1.0f + l * l, 2) * std::pow(m, 3)) +
                (std::pow(t, 5) * (3.0f * std::pow(d, 3) * g * std::pow(l, 3) * m - 3.0f * std::pow(d, 4) * l * l * (1.0f + l * l) * attackSpeed)) /
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

}