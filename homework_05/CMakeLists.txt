cmake_minimum_required(VERSION 3.20)
project(homework_05)

add_library(telemetry src/telemetry.cpp)
target_include_directories(telemetry PUBLIC include)

add_executable(telemetry_check src/main.cpp)
target_link_libraries(telemetry_check PRIVATE telemetry)