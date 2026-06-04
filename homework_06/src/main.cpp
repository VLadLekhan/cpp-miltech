#define _USE_MATH_DEFINES
#include "ballistics.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Warning: pleas write new file\n";
    std::cout << "Using: " << argv[0] << " <path to file>\n";
    return 1;
  }

  std::string filename = argv[1];
  std::ifstream input(filename);

  if (!input.is_open()) {
    std::cout << "File not open: " << filename << "\n";
    return 1;
  }

  float xd, yd, zd;
  float targetX, targetY;
  float attackSpeed, accelerationPath;
  std::string ammo_name;

  input >> xd >> yd >> zd >> targetX >> targetY >> attackSpeed >> accelerationPath >> ammo_name;

  try {
    ballistics::AmmoParams ammo = ballistics::get_ammo_params(ammo_name);

    ballistics::Calculation result = ballistics::calculate_drop_point(xd, yd, zd, targetX, targetY, attackSpeed, accelerationPath, ammo);

    std::ofstream output("output.txt");

    if (!output.is_open()) {
      std::cout << "File not open!" << std::endl;
    }

    if (result.has_inter_point) {
      output << result.inter.x << " " << result.inter.y << " " << result.fire.x << " " << result.fire.y << std::endl;
      std::cout << result.inter.x << " " << result.inter.y << std::endl;
      std::cout << result.fire.x << " " << result.fire.y << std::endl;
    }
    else {
      output << result.fire.x << " " << result.fire.y << std::endl;
      std::cout << result.fire.x << " " << result.fire.y << std::endl;
    }
  }
  catch (const std::exception& e) {
    std::cerr << "Critical problem is: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
