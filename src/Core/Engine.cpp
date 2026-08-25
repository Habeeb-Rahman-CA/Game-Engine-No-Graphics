#include "Core/Engine.hpp"
#include "Debug/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Engine {
namespace Core {

Engine::Engine(const EngineConfig& config)
    : m_state(EngineState::Uninitialized)
    , m_config(config)
    , m_time(config.fixedDeltaTime, config.targetFps)
    , m_input()
    , m_lastStatsTime(0.0)
    , m_lastFrameCount(0)
    , m_lastFixedUpdateCount(0)
    , m_verboseRender(config.showStats) {
}

Engine::~Engine() {
    if (m_state != EngineState::Shutdown && m_state != EngineState::Uninitialized) {
        shutdown();
    }
}

bool Engine::initialize() {
    LOG_INFO("==================================================");
    LOG_INFO("       BASIC GAME ENGINE - CORE PHASE 1           ");
    LOG_INFO("==================================================");
    
    m_state = EngineState::Initializing;
    LOG_INFO("Engine State Transition -> INITIALIZING");

    // Initialize subsystems
    m_time.init();
    m_input.init();

    m_state = EngineState::Running;
    LOG_INFO("Engine State Transition -> RUNNING");
    LOG_INFO("Target FPS: " + std::to_string(static_cast<int>(m_config.targetFps)) + 
             " | Fixed Update Hz: " + std::to_string(static_cast<int>(1.0 / m_config.fixedDeltaTime)));
    LOG_INFO("Controls: [Q] Quit | [P] Pause | [S] Toggle Stats | [+] Speed Up | [-] Slow Down | [H] Help");
    
    m_lastStatsTime = m_time.totalTime();
    return true;
}

void Engine::togglePause() {
    if (m_state == EngineState::Running) {
        m_state = EngineState::Paused;
        LOG_INFO("Engine State Transition -> PAUSED");
    } else if (m_state == EngineState::Paused) {
        m_state = EngineState::Running;
        LOG_INFO("Engine State Transition -> RESUMED (RUNNING)");
    }
}

void Engine::processInput() {
    m_input.pollInput();

    // Handle standard engine keyboard commands
    if (m_input.wasKeyJustPressed(KeyCode::Quit)) {
        LOG_INFO("Quit signal received from Input abstraction.");
        requestShutdown();
    }

    if (m_input.wasKeyJustPressed(KeyCode::Pause)) {
        togglePause();
    }

    if (m_input.wasKeyJustPressed(KeyCode::ToggleStats)) {
        m_verboseRender = !m_verboseRender;
        LOG_INFO(std::string("Stats display ") + (m_verboseRender ? "ENABLED" : "DISABLED"));
    }

    if (m_input.wasKeyJustPressed(KeyCode::SpeedUp)) {
        double current = m_time.targetFps();
        double next = current + 10.0;
        m_time.setTargetFps(next);
        LOG_INFO("Target FPS increased to: " + std::to_string(static_cast<int>(next)));
    }

    if (m_input.wasKeyJustPressed(KeyCode::SlowDown)) {
        double current = m_time.targetFps();
        double next = std::max(10.0, current - 10.0);
        m_time.setTargetFps(next);
        LOG_INFO("Target FPS decreased to: " + std::to_string(static_cast<int>(next)));
    }

    if (m_input.wasKeyJustPressed(KeyCode::CustomAction)) {
        LOG_INFO("Custom Action triggered!");
    }

    if (m_input.wasKeyJustPressed(KeyCode::Help)) {
        LOG_INFO("--- ENGINE HELP ---");
        LOG_INFO("  Q / ESC : Quit Engine");
        LOG_INFO("  P / Space : Pause / Resume Main Loop");
        LOG_INFO("  S : Toggle Periodic Render Statistics");
        LOG_INFO("  + / - : Adjust Target FPS");
        LOG_INFO("  A / Enter : Trigger Test Input Event");
    }
}

void Engine::update(double dt) {
    // Variable time step update logic
    // In future phases, system updates (movement, animations, audio) execute here.
    (void)dt;
}

void Engine::fixedUpdate(double fixedDt) {
    // Fixed time step update logic (Physics, fixed simulation step)
    // Runs at constant intervals regardless of frame rate dips/spikes.
    (void)fixedDt;
}

void Engine::render() {
    // Phase 1 - Since we're deliberately avoiding graphics,
    // render() prints periodic statistics to display engine performance metrics.
    if (!m_verboseRender) return;

    double currentTime = m_time.totalTime();
    if (currentTime - m_lastStatsTime >= m_config.statsIntervalSeconds) {
        double deltaSec = currentTime - m_lastStatsTime;
        uint64_t framesDelta = m_time.frameCount() - m_lastFrameCount;
        uint64_t fixedDelta = m_time.fixedUpdateCount() - m_lastFixedUpdateCount;

        double currentFps = static_cast<double>(framesDelta) / deltaSec;
        double currentFixedHz = static_cast<double>(fixedDelta) / deltaSec;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "[RENDER STATS] State: " << engineStateToString(m_state)
           << " | Time: " << currentTime << "s"
           << " | Frames: " << m_time.frameCount()
           << " | FPS: " << currentFps
           << " | Fixed Updates: " << m_time.fixedUpdateCount() << " (" << currentFixedHz << " Hz)"
           << " | dt: " << (m_time.deltaTime() * 1000.0) << " ms";

        LOG_INFO(ss.str());

        m_lastStatsTime = currentTime;
        m_lastFrameCount = m_time.frameCount();
        m_lastFixedUpdateCount = m_time.fixedUpdateCount();
    }
}

void Engine::run() {
    if (m_state != EngineState::Running && m_state != EngineState::Initializing) {
        LOG_ERROR("Engine must be initialized before calling run()");
        return;
    }

    LOG_INFO("Starting Game Engine Main Loop...");

    // =========================================================================
    // PHASE 1 MAIN GAME LOOP
    // =========================================================================
    // Initialize -> while(running) { process_input(), update(), fixed_update(), render() } -> Shutdown
    // =========================================================================
    
    while (m_state != EngineState::Shutdown) {
        // Step 1: Advance time system
        m_time.update();

        // Step 2: Check max frame limit if configured
        if (m_config.maxFramesToRun > 0 && m_time.frameCount() >= m_config.maxFramesToRun) {
            LOG_INFO("Reached target max frame count (" + std::to_string(m_config.maxFramesToRun) + "). Requesting shutdown.");
            requestShutdown();
            break;
        }

        // Step 3: Process Input
        processInput();

        // Step 4: Logic Updates (if not paused)
        if (m_state == EngineState::Running) {
            // Fixed rate updates (Physics / Fixed logic accumulator)
            while (m_time.checkFixedUpdate()) {
                fixedUpdate(m_time.fixedDeltaTime());
            }

            // Variable frame update (General game logic)
            update(m_time.deltaTime());
        }

        // Step 5: Render (Outputs statistics / frame state)
        render();

        // Step 6: Sleep / Frame pacing to hit target frame rate
        m_time.sleepIfNecessary();
    }

    shutdown();
}

void Engine::shutdown() {
    static bool shutdownCompleted = false;
    if (shutdownCompleted) return;
    shutdownCompleted = true;

    m_state = EngineState::Shutdown;
    LOG_INFO("Engine State Transition -> SHUTDOWN");
    LOG_INFO("Cleaning up resources and shutting down subsystems...");

    m_input.shutdown();

    LOG_INFO("==================================================");
    LOG_INFO("Engine Shutdown Complete.");
    LOG_INFO("Total Execution Time: " + std::to_string(m_time.totalTime()) + " s");
    LOG_INFO("Total Render Frames:  " + std::to_string(m_time.frameCount()));
    LOG_INFO("Total Fixed Ticks:    " + std::to_string(m_time.fixedUpdateCount()));
    LOG_INFO("Average FPS:          " + std::to_string(m_time.totalTime() > 0 ? static_cast<double>(m_time.frameCount()) / m_time.totalTime() : 0.0));
    LOG_INFO("==================================================");
}

} // namespace Core
} // namespace Engine
