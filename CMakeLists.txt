cmake_minimum_required(VERSION 3.24)
project(rpg_world_simulator)


set(RPG_WORLD_SIMULATOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)


add_executable(rpg_world_simulator ${RPG_WORLD_SIMULATOR_SOURCES})
target_include_directories(rpg_world_simulator
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
