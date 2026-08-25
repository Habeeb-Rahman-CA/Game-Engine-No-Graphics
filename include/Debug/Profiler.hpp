#ifndef DEBUG_PROFILER_HPP
#define DEBUG_PROFILER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <cstddef>
#include <atomic>

namespace Engine {
namespace Debug {

struct ProfileRecord {
    std::string name;
    double durationMs;
};

class Profiler {
public:
    static Profiler& getInstance() {
        static Profiler instance;
        return instance;
    }

    void beginFrame();
    void endFrame();
    void recordTiming(const std::string& name, double durationMs);
    void recordAllocation(size_t bytes);

    void printFrameReport() const;
    std::string getReportString() const;

    double getFrameTimeMs() const { return m_lastFrameTimeMs; }
    size_t getAllocationCount() const { return m_lastFrameAllocations; }

private:
    Profiler();
    ~Profiler() = default;

    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint m_frameStartTime;
    double m_lastFrameTimeMs = 0.0;
    
    std::atomic<size_t> m_currentFrameAllocations{0};
    std::atomic<size_t> m_currentFrameAllocatedBytes{0};
    size_t m_lastFrameAllocations = 0;
    size_t m_lastFrameAllocatedBytes = 0;

    std::unordered_map<std::string, double> m_currentTimings;
    std::vector<std::string> m_timingOrder;
    std::unordered_map<std::string, double> m_lastTimings;
};

class ProfileScope {
public:
    explicit ProfileScope(const std::string& name)
        : m_name(name), m_startTime(std::chrono::high_resolution_clock::now()) {}

    ~ProfileScope() {
        auto endTime = std::chrono::high_resolution_clock::now();
        double durationMs = std::chrono::duration<double, std::milli>(endTime - m_startTime).count();
        Profiler::getInstance().recordTiming(m_name, durationMs);
    }

private:
    std::string m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

} // namespace Debug
} // namespace Engine

#define PROFILE_SCOPE(name) ::Engine::Debug::ProfileScope profileScope_##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

#endif // DEBUG_PROFILER_HPP
