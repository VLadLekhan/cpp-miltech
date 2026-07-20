cmake_minimum_required(VERSION 3.20)
project(homework_07)

add_executable(homework_07_bin
src/main.cpp
src/implementations.cpp
src/MissionProcessor.cpp
)

target_include_directories(homework_07_bin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)