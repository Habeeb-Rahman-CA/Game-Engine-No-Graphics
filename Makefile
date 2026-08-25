CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -O2
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game_engine

SOURCES = main.cpp \
          src/Core/Engine.cpp \
          src/Core/Time.cpp \
          src/Core/Input.cpp \
          src/Input/TerminalInputBackend.cpp \
          src/Input/Input.cpp \
          src/State/StateManager.cpp \
          src/State/ConcreteStates.cpp \
          src/World/World.cpp \
          src/Physics/Collision.cpp \
          src/System/MovementSystem.cpp \
          src/System/PhysicsSystem.cpp \
          src/System/GameplaySystem.cpp \
          src/Memory/ArenaAllocator.cpp \
          src/Memory/PoolAllocator.cpp \
          src/Memory/MemoryBenchmark.cpp \
          src/Resources/Resource.cpp \
          src/Event/EventBus.cpp \
          src/Debug/Profiler.cpp \
          src/Debug/Logger.cpp

OBJECTS = $(patsubcast %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

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

perf-profile: $(TARGET)
	perf stat ./$(TARGET) --frames 500 --fps 120

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run test test-phase2 test-phase3 test-phase4 test-phase5 test-phase6 test-phase7 test-phase8 test-phase9 test-profiler test-math test-input test-state test-collision test-physics perf-profile clean
