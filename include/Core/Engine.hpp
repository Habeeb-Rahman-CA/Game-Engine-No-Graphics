#ifndef CORE_ENGINE_HPP
#define CORE_ENGINE_HPP

#include "Core/EngineState.hpp"
#include "Core/Time.hpp"
#include "Core/Input.hpp"
#include <cstdint>

namespace Engine {
namespace Core {

struct EngineConfig {
    double targetFps = 60.0;
    double fixedDeltaTime = 1.0 / 60.0;
    uint64_t maxFramesToRun = 0; // 0 = infinite (run until quit)
    bool showStats = true;
    double statsIntervalSeconds = 1.0;
};

class Engine {
public:
    Engine(const EngineConfig& config = EngineConfig());
    ~Engine();

    // Engine Lifecycle Methods
    bool initialize();
    void run();
    void shutdown();

    // Getters & Setters
    EngineState getState() const { return m_state; }
    Time& getTime() { return m_time; }
    const Time& getTime() const { return m_time; }
    Input& getInput() { return m_input; }
    const Input& getInput() const { return m_input; }

    void requestShutdown() { m_state = EngineState::Shutdown; }
    void togglePause();

private:
    // Core Game Loop Functions
    void processInput();
    void update(double dt);
    void fixedUpdate(double fixedDt);
    void render();

private:
    EngineState m_state;
    EngineConfig m_config;
    Time m_time;
    Input m_input;

    // Statistics tracking for render() output
    double m_lastStatsTime;
    uint64_t m_lastFrameCount;
    uint64_t m_lastFixedUpdateCount;
    bool m_verboseRender;
};

} // namespace Core
} // namespace Engine

#endif // CORE_ENGINE_HPP
