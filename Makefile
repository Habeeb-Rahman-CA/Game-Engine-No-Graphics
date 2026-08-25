CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -O2
LDFLAGS = -lX11
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game_engine

SOURCES = main.cpp \
          src/Core/Engine.cpp \
          src/Core/Time.cpp \
          src/Core/Input.cpp \
          src/Input/TerminalInputBackend.cpp \
          src/Input/Input.cpp \
          src/Renderer/Renderer.cpp \
          src/Renderer/TerminalCanvasRenderer.cpp \
          src/Audio/Audio.cpp \
          src/State/StateManager.cpp \
          src/State/ConcreteStates.cpp \
          src/World/World.cpp \
          src/Physics/Collision.cpp \
          src/System/MovementSystem.cpp \
          src/System/PhysicsSystem.cpp \
          src/System/GameplaySystem.cpp \
          src/System/RenderSystem.cpp \
          src/System/AnimationSystem.cpp \
          src/System/AISystem.cpp \
          src/Memory/ArenaAllocator.cpp \
          src/Memory/PoolAllocator.cpp \
          src/Memory/MemoryBenchmark.cpp \
          src/Resources/Resource.cpp \
          src/Event/EventBus.cpp \
          src/Debug/Profiler.cpp \
          src/Debug/Logger.cpp \
          src/Debug/DebugRenderer.cpp \
          src/Scene/SceneSerializer.cpp \
          src/Platform/FileSystem.cpp \
          src/Platform/Window.cpp \
          src/Platform/Platform.cpp \
          src/Renderer/WindowCanvasRenderer.cpp \
          src/Renderer/GPU2DRenderer.cpp \
          src/Physics/SpatialGrid.cpp \
          src/UI/HUDSystem.cpp \
          src/World/Tilemap.cpp \
          src/Editor/Editor.cpp

OBJECTS = $(patsubcast %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

SNAKE_SOURCES = SnakeGame/src/main.cpp \
                src/Core/Engine.cpp \
                src/Core/Time.cpp \
                src/Core/Input.cpp \
                src/Input/TerminalInputBackend.cpp \
                src/Input/Input.cpp \
                src/Renderer/Renderer.cpp \
                src/Renderer/TerminalCanvasRenderer.cpp \
                src/Audio/Audio.cpp \
                src/State/StateManager.cpp \
                src/State/ConcreteStates.cpp \
                src/World/World.cpp \
                src/Physics/Collision.cpp \
                src/System/MovementSystem.cpp \
                src/System/PhysicsSystem.cpp \
                src/System/GameplaySystem.cpp \
                src/System/RenderSystem.cpp \
                src/System/AnimationSystem.cpp \
                src/System/AISystem.cpp \
                src/Memory/ArenaAllocator.cpp \
                src/Memory/PoolAllocator.cpp \
                src/Memory/MemoryBenchmark.cpp \
                src/Resources/Resource.cpp \
                src/Event/EventBus.cpp \
                src/Debug/Profiler.cpp \
                src/Debug/Logger.cpp \
                src/Debug/DebugRenderer.cpp \
                src/Scene/SceneSerializer.cpp \
                src/Platform/FileSystem.cpp \
                src/Platform/Window.cpp \
                src/Platform/Platform.cpp \
                src/Renderer/WindowCanvasRenderer.cpp \
                src/Renderer/GPU2DRenderer.cpp \
                src/Physics/SpatialGrid.cpp \
                src/UI/HUDSystem.cpp \
                src/World/Tilemap.cpp

snake_game: $(SNAKE_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SNAKE_SOURCES) $(LDFLAGS) -o $(BUILD_DIR)/snake_game

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) --frames 180 --fps 60

test-phase2: $(TARGET)
	./$(TARGET) --phase2

test-phase3: $(TARGET)
	./$(TARGET) --phase3

test-phase4: $(TARGET)
	./$(TARGET) --phase4

test-phase5: $(TARGET)
	./$(TARGET) --phase5

test-phase6: $(TARGET)
	./$(TARGET) --phase6

test-phase7: $(TARGET)
	./$(TARGET) --phase7

test-phase8: $(TARGET)
	./$(TARGET) --phase8

test-phase9: $(TARGET)
	./$(TARGET) --phase9

test-profiler: $(TARGET)
	./$(TARGET) --profiler

test-math: $(TARGET)
	./$(TARGET) --math

test-input: $(TARGET)
	./$(TARGET) --input

test-state: $(TARGET)
	./$(TARGET) --state

test-collision: $(TARGET)
	./$(TARGET) --collision

test-physics: $(TARGET)
	./$(TARGET) --physics

test-render: $(TARGET)
	./$(TARGET) --render

test-camera: $(TARGET)
	./$(TARGET) --camera

test-sprite: $(TARGET)
	./$(TARGET) --sprite

test-audio: $(TARGET)
	./$(TARGET) --audio

test-debug: $(TARGET)
	./$(TARGET) --debug-tools

test-scene: $(TARGET)
	./$(TARGET) --scene

test-ai: $(TARGET)
	./$(TARGET) --ai

test-window: $(TARGET)
	./$(TARGET) --window

test-gpu: $(TARGET)
	./$(TARGET) --gpu

TEST_SOURCES = tests/test_runner.cpp \
               tests/math_tests.cpp \
               tests/allocator_tests.cpp \
               tests/entity_tests.cpp \
               tests/collision_tests.cpp \
               tests/event_tests.cpp \
               tests/serialization_tests.cpp \
               src/Core/Time.cpp \
               src/World/World.cpp \
               src/Physics/Collision.cpp \
               src/Memory/ArenaAllocator.cpp \
               src/Memory/PoolAllocator.cpp \
               src/Resources/Resource.cpp \
               src/Event/EventBus.cpp \
               src/Debug/Profiler.cpp \
               src/Debug/Logger.cpp \
               src/Scene/SceneSerializer.cpp \
               src/Platform/FileSystem.cpp \
               src/Platform/Window.cpp \
               src/Platform/Platform.cpp \
               src/Renderer/WindowCanvasRenderer.cpp \
               src/Renderer/GPU2DRenderer.cpp

test-engine:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) $(LDFLAGS) -o $(BUILD_DIR)/engine_tests
	./$(BUILD_DIR)/engine_tests

BENCHMARK_SOURCES = benchmarks/benchmark_runner.cpp \
                    src/World/World.cpp \
                    src/Physics/Collision.cpp \
                    src/System/MovementSystem.cpp \
                    src/Memory/ArenaAllocator.cpp \
                    src/Memory/PoolAllocator.cpp \
                    src/Resources/Resource.cpp \
                    src/Event/EventBus.cpp \
                    src/Debug/Profiler.cpp \
                    src/Debug/Logger.cpp \
                    src/Scene/SceneSerializer.cpp \
                    src/Platform/FileSystem.cpp \
                    src/Platform/Window.cpp \
                    src/Platform/Platform.cpp \
                    src/Renderer/WindowCanvasRenderer.cpp \
                    src/Renderer/GPU2DRenderer.cpp

benchmark:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(BENCHMARK_SOURCES) $(LDFLAGS) -o $(BUILD_DIR)/engine_benchmarks
	./$(BUILD_DIR)/engine_benchmarks

benchmark-perf: benchmark
	perf stat ./$(BUILD_DIR)/engine_benchmarks

perf-profile: $(TARGET)
	perf stat ./$(TARGET) --frames 500 --fps 120

DUNGEON_SOURCES = DungeonGame/src/main.cpp \
                  src/Core/Engine.cpp \
                  src/Core/Time.cpp \
                  src/Core/Input.cpp \
                  src/Input/TerminalInputBackend.cpp \
                  src/Input/Input.cpp \
                  src/Renderer/Renderer.cpp \
                  src/Renderer/TerminalCanvasRenderer.cpp \
                  src/Audio/Audio.cpp \
                  src/State/StateManager.cpp \
                  src/State/ConcreteStates.cpp \
                  src/World/World.cpp \
                  src/Physics/Collision.cpp \
                  src/System/MovementSystem.cpp \
                  src/System/PhysicsSystem.cpp \
                  src/System/GameplaySystem.cpp \
                  src/System/RenderSystem.cpp \
                  src/System/AnimationSystem.cpp \
                  src/System/AISystem.cpp \
                  src/Memory/ArenaAllocator.cpp \
                  src/Memory/PoolAllocator.cpp \
                  src/Memory/MemoryBenchmark.cpp \
                  src/Resources/Resource.cpp \
                  src/Event/EventBus.cpp \
                  src/Debug/Profiler.cpp \
                  src/Debug/Logger.cpp \
                  src/Debug/DebugRenderer.cpp \
                  src/Scene/SceneSerializer.cpp \
                  src/Platform/FileSystem.cpp \
                  src/Platform/Window.cpp \
                  src/Platform/Platform.cpp \
                  src/Renderer/WindowCanvasRenderer.cpp \
                  src/Renderer/GPU2DRenderer.cpp

run-dungeon-game:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DUNGEON_SOURCES) $(LDFLAGS) -o $(BUILD_DIR)/dungeon_game
	./$(BUILD_DIR)/dungeon_game

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run test test-phase2 test-phase3 test-phase4 test-phase5 test-phase6 test-phase7 test-phase8 test-phase9 test-profiler test-math test-input test-state test-collision test-physics test-render test-camera test-sprite test-audio test-debug test-scene test-ai test-engine benchmark benchmark-perf perf-profile clean
