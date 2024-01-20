cmake_minimum_required(VERSION 3.24)
project(rpg_world_simulator)


add_subdirectory(ext)


set(RPG_WORLD_SIMULATOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FileUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SpriteRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SpriteSheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Viewport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
)


add_executable(rpg_world_simulator ${RPG_WORLD_SIMULATOR_SOURCES})
target_include_directories(rpg_world_simulator
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(rpg_world_simulator
    PUBLIC  gut_opengl
)
target_compile_definitions(rpg_world_simulator
    PUBLIC  SHADER_DIR="\"\"${CMAKE_CURRENT_SOURCE_DIR}/shaders/\"\""
    PUBLIC  ASSETS_DIR="\"\"${CMAKE_CURRENT_SOURCE_DIR}/assets/\"\""
)