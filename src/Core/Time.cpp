#include "Core/Time.hpp"
#include <thread>
#include <algorithm>

namespace Engine {
namespace Core {

Time::Time(double fixedDeltaTimeSeconds, double targetFps)
    : m_deltaTime(0.0)
    , m_fixedDeltaTime(fixedDeltaTimeSeconds)
    , m_totalTime(0.0)
    , m_accumulator(0.0)
    , m_targetFps(targetFps)
    , m_frameCount(0)
    , m_fixedUpdateCount(0)
    , m_fpsTimer(0.0)
    , m_fpsFrames(0)
    , m_fps(0.0) {
}

void Time::init() {
    m_startTime = Clock::now();
    m_lastFrameTime = m_startTime;
    m_currentFrameTime = m_startTime;
    m_deltaTime = 0.0;
    m_totalTime = 0.0;
    m_accumulator = 0.0;
    m_frameCount = 0;
    m_fixedUpdateCount = 0;
    m_fpsTimer = 0.0;
    m_fpsFrames = 0;
    m_fps = 0.0;
}

void Time::update() {
    m_currentFrameTime = Clock::now();
    std::chrono::duration<double> elapsed = m_currentFrameTime - m_lastFrameTime;
    m_lastFrameTime = m_currentFrameTime;

    m_deltaTime = elapsed.count();
    
    // Prevent spiral of death on large lags (cap max delta time to 0.25s)
    if (m_deltaTime > 0.25) {
        m_deltaTime = 0.25;
    }

    m_totalTime += m_deltaTime;
    m_accumulator += m_deltaTime;
    m_frameCount++;

    // Calculate smoothed FPS
    m_fpsTimer += m_deltaTime;
    m_fpsFrames++;
    if (m_fpsTimer >= 1.0) {
        m_fps = static_cast<double>(m_fpsFrames) / m_fpsTimer;
        m_fpsTimer = 0.0;
        m_fpsFrames = 0;
    }
}

bool Time::checkFixedUpdate() {
    if (m_accumulator >= m_fixedDeltaTime) {
        m_accumulator -= m_fixedDeltaTime;
        m_fixedUpdateCount++;
        return true;
    }
    return false;
}

void Time::sleepIfNecessary() {
    if (m_targetFps <= 0.0) return;

    double targetFrameDuration = 1.0 / m_targetFps;
    auto now = Clock::now();
    std::chrono::duration<double> frameElapsed = now - m_currentFrameTime;
    
    if (frameElapsed.count() < targetFrameDuration) {
        double sleepSeconds = targetFrameDuration - frameElapsed.count();
        std::this_thread::sleep_for(std::chrono::duration<double>(sleepSeconds));
    }
}

} // namespace Core
} // namespace Engine
