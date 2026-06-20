cmake_minimum_required(VERSION 3.20)
project(homework_08)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces
${CMAKE_CURRENT_SOURCE_DIR}/include/providers
${CMAKE_CURRENT_SOURCE_DIR}/include/config
${CMAKE_CURRENT_SOURCE_DIR}/include/solvers
)


set(SOURCES 
src/main.cpp
src/MissionProcessor.cpp
src/providers/JsonTargetProvider.cpp
src/solvers/AnalyticalSolver.cpp      
src/config/FileConfigLoader.cpp
)

add_executable(homework_08_bin ${SOURCES})

