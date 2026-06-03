#include <gtest/gtest.h>
#include <stdexcept>
#include "ballistics.hpp"

TEST(BallisticsLookup, GetAmmoParamsValid)
{
  auto params = ballistics::get_ammo_params("VOG-17");
  EXPECT_FLOAT_EQ(params.m, 0.35f);
  EXPECT_FLOAT_EQ(params.d, 0.07f);
}

TEST(Ballistics, ComputesKnownDropPoint)
{
  const float drone_x = 100.0f;
  const float drone_y = 100.0f;
  const float drone_z = 100.0f;
  const float target_x = 200.0f;
  const float target_y = 200.0f;
  const float speed = 10.0f;
  const float path = 10.0f;
  const auto ammo = ballistics::get_ammo_params("VOG-17");

  const auto result = ballistics::calculate_drop_point(drone_x, drone_y, drone_z, target_x, target_y, speed, path, ammo);

  EXPECT_NEAR(result.fire.x, 173.759f, 0.01f);
  EXPECT_NEAR(result.fire.y, 173.759f, 0.01f);
}

TEST(BallisticsLookup, GetInvalidName)
{
  EXPECT_THROW(ballistics::get_ammo_params("UNKNOWN"), std::runtime_error);
}

TEST(BallisticsMath, CalculationBasics)
{
  ballistics::AmmoParams ammo{0.35f, 0.07f, 0.0f};
  auto result = ballistics::calculate_drop_point(0, 0, 100, 500, 500, 20, 10, ammo);
  EXPECT_NE(result.fire.x, 0.0f);
  EXPECT_NE(result.fire.y, 0.0f);
}