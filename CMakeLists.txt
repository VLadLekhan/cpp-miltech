cmake_minimum_required(VERSION 3.20)
project(section2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,readability-*,bugprone-*")

enable_testing()

# Домашні роботи. По мірі появи додаємо кожну окремим add_subdirectory.
#add_subdirectory(homework_04)
add_subdirectory(homework_06)
