#define _USE_MATH_DEFINES
#include "ballistics.hpp"
#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>

int main()
{
  std::ifstream input("input.txt");

  if (!input.is_open()) {
    std::cout << "File not open!";
    return 2;
  }
  float xd, yd, zd;
  float targetX, targetY;
  float attackSpeed, accelerationPath;
  char ammo_name[15];

  input >> xd >> yd >> zd >> targetX >> targetY >> attackSpeed >> accelerationPath >> ammo_name;

  // std::cout << xd << " " << yd << " " << zd << " " << targetX << " " << targetY << " " << attackSpeed << " " << accelerationPath << " "
  // << ammo_name << " " << std::endl;

  //float m = 0, d = 0, l = 0;

  ballistics::AmmoParams ammo = ballistics::get_ammo_params(ammo_name);

  
  float a, b, c;
  float g = 9.81;

  a = (d * g * m) - (2.0 * d * d * l * attackSpeed);
  b = (-3.0 * g * m * m) + (3.0 * d * l * m * attackSpeed);
  c = 6.0 * m * m * zd;

  float p, q;

  p = -(b * b) / (3.0 * a * a);
  q = (2.0 * b * b * b) / (27.0 * a * a * a) + (c / a);

  float arg_arccos = ((3.0 * q) / (2.0 * p)) * std::sqrt(-3.0 / p);

  if (arg_arccos < -1.0 || arg_arccos > 1.0) {
    std::cerr << "CRITICAL ERROR: Physics model failure." << "Argument for arccos(" << arg_arccos << ") is out of domain[-1, 1]."
              << std::endl;
    return 1;
  }

  float phi = std::acos(arg_arccos);

  float t;
  float pi = M_PI;

  t = (2.0 * std::sqrt(-p / 3.0)) * std::cos((phi + 4.0 * pi) / 3.0) - b / (3.0 * a);

  double h =
    (attackSpeed * t) - (t * t * d * attackSpeed) / (2.0 * m) +
    (std::pow(t, 3) * (6.0 * d * g * l * m - 6.0 * d * d * (l * l - 1.0) * attackSpeed)) / (36.0 * m * m) +
    (std::pow(t, 4) *
     (-6.0 * d * d * g * l * (1.0 + l * l + std::pow(l, 4)) * m + 3.0 * std::pow(d, 3) * l * l * (1.0 + l * l) * attackSpeed +
      6.0 * std::pow(d, 3) * std::pow(l, 4) * (1.0 + l * l) * attackSpeed)) /
      (36.0 * std::pow(1.0 + l * l, 2) * std::pow(m, 3)) +
    (std::pow(t, 5) * (3.0 * std::pow(d, 3) * g * std::pow(l, 3) * m - 3.0 * std::pow(d, 4) * l * l * (1.0 + l * l) * attackSpeed)) /
      (36.0 * (1.0 + l * l) * std::pow(m, 4));

  float D = std::sqrt(std::pow(targetX - xd, 2) + std::pow(targetY - yd, 2));

  if (D < 0.001f) {
    std::cout << "Error: Target distance is zero. Cannot compute approach vector." << std::endl;
  }

  float fireX, fireY, interX, interY;
  float ratio = (D - h) / D;

  if (h + accelerationPath > D) {
    interX = targetX - (targetX - xd) * (h + accelerationPath) / D;
    interY = targetY - (targetY - yd) * (h + accelerationPath) / D;
  }
  fireX = xd + (targetX - xd) * ratio;
  fireY = yd + (targetY - yd) * ratio;

  std::ofstream output("output2.txt");

  if (!output.is_open()) {
    std::cout << "File not open!" << std::endl;
  }

  if (h + accelerationPath > D) {
    std::cout << interX << " " << interY << std::endl;
    std::cout << fireX << " " << fireY << std::endl;
  }
  else {
    std::cout << fireX << " " << fireY << std::endl;
  }

  return 0;
}
