#include "Debug/Logger.hpp"
#include <chrono>
#include <iomanip>
#include <ctime>

namespace Engine {
namespace Debug {

LogLevel Logger::s_minLevel = LogLevel::Info;

void Logger::setLogLevel(LogLevel level) {
    s_minLevel = level;
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error:   return "ERROR";
    }
    return "UNKNOWN";
}

std::string Logger::levelToColor(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "\033[36m"; // Cyan
        case LogLevel::Info:    return "\033[32m"; // Green
        case LogLevel::Warning: return "\033[33m"; // Yellow
        case LogLevel::Error:   return "\033[31m"; // Red
    }
    return "\033[0m";
}

std::string Logger::getCurrentTimeString() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    std::tm tm_buf;
    localtime_r(&time_t_now, &tm_buf);
    ss << std::put_time(&tm_buf, "%H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    if (level < s_minLevel) return;

    std::stringstream ss;
    ss << "[" << getCurrentTimeString() << "] "
       << levelToColor(level) << "[" << levelToString(level) << "]\033[0m ";

    if (file && (level == LogLevel::Error || level == LogLevel::Warning || level == LogLevel::Debug)) {
        std::string filename(file);
        size_t pos = filename.find_last_of("/\\");
        if (pos != std::string::npos) {
            filename = filename.substr(pos + 1);
        }
        ss << "(" << filename << ":" << line << ") ";
    }

    ss << message << "\n";
    std::cout << ss.str() << std::flush;
}

} // namespace Debug
} // namespace Engine
