#include "Debug/Profiler.hpp"
#include "Debug/Logger.hpp"
#include <iomanip>
#include <sstream>

namespace Engine {
namespace Debug {

Profiler::Profiler() {
    beginFrame();
}

void Profiler::beginFrame() {
    m_frameStartTime = Clock::now();
    m_currentTimings.clear();
    m_timingOrder.clear();
    m_currentFrameAllocations = 0;
    m_currentFrameAllocatedBytes = 0;
}

void Profiler::endFrame() {
    auto endTime = Clock::now();
    m_lastFrameTimeMs = std::chrono::duration<double, std::milli>(endTime - m_frameStartTime).count();
    m_lastTimings = m_currentTimings;
    m_lastFrameAllocations = m_currentFrameAllocations.load();
    m_lastFrameAllocatedBytes = m_currentFrameAllocatedBytes.load();
}

void Profiler::recordTiming(const std::string& name, double durationMs) {
    if (m_currentTimings.find(name) == m_currentTimings.end()) {
        m_timingOrder.push_back(name);
        m_currentTimings[name] = durationMs;
    } else {
        m_currentTimings[name] += durationMs;
    }
}

void Profiler::recordAllocation(size_t bytes) {
    m_currentFrameAllocations++;
    m_currentFrameAllocatedBytes += bytes;
}

std::string Profiler::getReportString() const {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    ss << "\n================ ENGINE PROFILER REPORT ================\n";
    ss << "Frame Time : " << m_lastFrameTimeMs << " ms\n";
    ss << "Allocations: " << m_lastFrameAllocations << " (" << m_lastFrameAllocatedBytes << " bytes)\n";
    ss << "--------------------------------------------------------\n";

    double calculatedTotal = 0.0;
    for (const auto& name : m_timingOrder) {
        auto it = m_lastTimings.find(name);
        if (it != m_lastTimings.end()) {
            double ms = it->second;
            calculatedTotal += ms;
            ss << std::left << std::setw(14) << name << ": " 
               << std::right << std::setw(6) << std::setprecision(2) << ms << " ms\n";
        }
    }
    ss << "--------------------------------------------------------\n";
    ss << std::left << std::setw(14) << "Total Tracked" << ": " 
       << std::right << std::setw(6) << std::setprecision(2) << calculatedTotal << " ms\n";
    ss << "========================================================\n";

    return ss.str();
}

void Profiler::printFrameReport() const {
    LOG_INFO(getReportString());
}

} // namespace Debug
} // namespace Engine
