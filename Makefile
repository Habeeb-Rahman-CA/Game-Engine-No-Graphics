CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -O2
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game_engine

SOURCES = main.cpp \
          src/Core/Engine.cpp \
          src/Core/Time.cpp \
          src/Core/Input.cpp \
          src/World/World.cpp \
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

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run test test-phase2 test-phase3 test-phase4 test-phase5 clean
