#define _USE_MATH_DEFINES
#include "ballistics.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace ballistics {

AmmoParams get_ammo_params(const std::string& ammo_name)
{
  if (ammo_name == "VOG-17")
    return {0.35f, 0.07f, 0.0f};
  if (ammo_name == "M67")
    return {0.6f, 0.10f, 0.0f};
  if (ammo_name == "RKG-3")
    return {1.2f, 0.10f, 0.0f};
  if (ammo_name == "GLIDING-VOG")
    return {0.45f, 0.10f, 1.0f};
  if (ammo_name == "GLIDING-RKG")
    return {1.4f, 0.10f, 1.0f};

  throw std::runtime_error("Unknown type:  " + ammo_name);
}

Calculation calculate_drop_point(
  float xd, float yd, float zd, float targetX, float targetY, float attackSpeed, float accelerationPath, const AmmoParams& ammo_name)
{
  float g = 9.81f;
  float m = ammo_name.m;
  float d = ammo_name.d;
  float l = ammo_name.l;

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

  float t;
  float pi = M_PI;

  t = (2.0f * std::sqrt(-p / 3.0f)) * std::cos((phi + 4.0f * pi) / 3.0f) - b / (3.0f * a);

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

  Calculation result;
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
};

}  // namespace ballistics