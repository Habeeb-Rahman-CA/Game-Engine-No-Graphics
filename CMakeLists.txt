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
    src/Input/TerminalInputBackend.cpp
    src/Input/Input.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/TerminalCanvasRenderer.cpp
    src/Audio/Audio.cpp
    src/State/StateManager.cpp
    src/State/ConcreteStates.cpp
    src/World/World.cpp
    src/Physics/Collision.cpp
    src/System/MovementSystem.cpp
    src/System/PhysicsSystem.cpp
    src/System/GameplaySystem.cpp
    src/System/RenderSystem.cpp
    src/System/AnimationSystem.cpp
    src/System/AISystem.cpp
    src/Memory/ArenaAllocator.cpp
    src/Memory/PoolAllocator.cpp
    src/Memory/MemoryBenchmark.cpp
    src/Resources/Resource.cpp
    src/Event/EventBus.cpp
    src/Debug/Profiler.cpp
    src/Debug/Logger.cpp
    src/Debug/DebugRenderer.cpp
    src/Scene/SceneSerializer.cpp
    src/Platform/FileSystem.cpp
    src/Platform/Window.cpp
    src/Platform/Platform.cpp
    src/Renderer/WindowCanvasRenderer.cpp
    src/Renderer/GPU2DRenderer.cpp
    src/Editor/Editor.cpp
)

# Find & Link X11
find_package(X11 REQUIRED)

# Engine library
add_library(EngineCore STATIC ${ENGINE_SOURCES})
target_link_libraries(EngineCore PRIVATE ${X11_LIBRARIES})

# Executable
add_executable(game_engine main.cpp)
target_link_libraries(game_engine PRIVATE EngineCore)

# Test Runner Executable
add_executable(engine_tests
    tests/test_runner.cpp
    tests/math_tests.cpp
    tests/allocator_tests.cpp
    tests/entity_tests.cpp
    tests/collision_tests.cpp
    tests/event_tests.cpp
    tests/serialization_tests.cpp
)
target_link_libraries(engine_tests PRIVATE EngineCore)

# Benchmark Executable
add_executable(engine_benchmarks benchmarks/benchmark_runner.cpp)
target_link_libraries(engine_benchmarks PRIVATE EngineCore)

# Dungeon Game Executable (Public SDK Demo)
add_executable(dungeon_game DungeonGame/src/main.cpp)
target_link_libraries(dungeon_game PRIVATE EngineCore ${X11_LIBRARIES})

# Target options
target_compile_options(game_engine PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(engine_tests PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(engine_benchmarks PRIVATE -Wall -Wextra -Wpedantic)
