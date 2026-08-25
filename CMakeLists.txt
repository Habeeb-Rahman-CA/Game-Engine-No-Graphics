cmake_minimum_required(VERSION 3.16)
project(BasicGameEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(ENGINE_SOURCES
    src/Core/Engine.cpp
    src/Core/Time.cpp
    src/Core/Input.cpp
    src/World/World.cpp
    src/System/MovementSystem.cpp
    src/System/PhysicsSystem.cpp
    src/System/GameplaySystem.cpp
    src/Debug/Logger.cpp
)

# Engine library
add_library(EngineCore STATIC ${ENGINE_SOURCES})

# Executable
add_executable(game_engine main.cpp)
target_link_libraries(game_engine PRIVATE EngineCore)

# Target options
target_compile_options(game_engine PRIVATE -Wall -Wextra -Wpedantic)
