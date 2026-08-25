#ifndef CORE_TIME_HPP
#define CORE_TIME_HPP

#include <chrono>

namespace Engine {
namespace Core {

class Time {
public:
    Time(double fixedDeltaTimeSeconds = 1.0 / 60.0, double targetFps = 60.0);

    void init();
    void update();
    bool checkFixedUpdate();
    void sleepIfNecessary();

    // =========================================================================
    // PHASE 2 TIME SYSTEM MODULE ATTRIBUTES
    // Time: delta_time, total_time, frame_count, FPS
    // =========================================================================
    double delta_time() const { return m_deltaTime; }
    double total_time() const { return m_totalTime; }
    unsigned long long frame_count() const { return m_frameCount; }
    double FPS() const { return m_fps; }

    // Standard CamelCase getters
    double deltaTime() const { return m_deltaTime; }
    double fixedDeltaTime() const { return m_fixedDeltaTime; }
    double totalTime() const { return m_totalTime; }
    double fps() const { return m_fps; }
    unsigned long long frameCount() const { return m_frameCount; }
    unsigned long long fixedUpdateCount() const { return m_fixedUpdateCount; }
    double targetFps() const { return m_targetFps; }

    // =========================================================================
    // PHASE 3 FIXED TIMESTEP & INTERPOLATION
    // =========================================================================
    double accumulator() const { return m_accumulator; }
    double getInterpolationAlpha() const { 
        return (m_fixedDeltaTime > 0.0) ? (m_accumulator / m_fixedDeltaTime) : 0.0; 
    }

    // Setters
    void setFixedDeltaTime(double seconds) { m_fixedDeltaTime = seconds; }
    void setTargetFps(double fps) { m_targetFps = fps; }

private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint m_startTime;
    TimePoint m_lastFrameTime;
    TimePoint m_currentFrameTime;

    double m_deltaTime;
    double m_fixedDeltaTime;
    double m_totalTime;
    double m_accumulator;

    double m_targetFps;
    unsigned long long m_frameCount;
    unsigned long long m_fixedUpdateCount;

    // FPS calculation window
    double m_fpsTimer;
    unsigned int m_fpsFrames;
    double m_fps;
};

} // namespace Core
} // namespace Engine

#endif // CORE_TIME_HPP
