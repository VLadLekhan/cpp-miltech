//#include <execution>
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
    return 1;
  }
  float xd, yd, zd;
  float targetX, targetY;
  float attackSpeed, accelerationPath;
 std::string ammo_name;

  input >> xd >> yd >> zd >> targetX >> targetY >> attackSpeed >> accelerationPath >> ammo_name;

  try {

    ballistics::AmmoParams ammo = ballistics::get_ammo_params(ammo_name);

    ballistics::Calculation result = ballistics::calculate_drop_point( 
    xd, yd, zd, 
    targetX, targetY, 
    attackSpeed, accelerationPath, 
    ammo);

    std::ofstream output("output.txt");

    if (!output.is_open()) {
      std::cout << "File not open!" << std::endl;
    }
  

    if (result.has_inter_point) {
      output << result.inter.x << " " << result.inter.y << " " << result.fire.x << " " << result.fire.y <<std::endl;
      std::cout << result.inter.x << " " << result.inter.y << std::endl;
      std::cout << result.fire.x << " " << result.fire.y << std::endl;
    }
    else {
      std::cout << result.fire.x << " " << result.fire.y << std::endl;
    }

  } catch (const std::exception& e) {
      std::cerr << "Critical problem is: " << e.what() << std::endl;
      return 1;
  }


  return 0;
}
