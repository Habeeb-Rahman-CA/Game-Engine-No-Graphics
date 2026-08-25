#ifndef DEBUG_LOGGER_HPP
#define DEBUG_LOGGER_HPP

#include <string>
#include <iostream>
#include <sstream>

namespace Engine {
namespace Debug {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static void setLogLevel(LogLevel level);
    static void log(LogLevel level, const std::string& message, const char* file = nullptr, int line = 0);

private:
    static LogLevel s_minLevel;
    static std::string levelToString(LogLevel level);
    static std::string levelToColor(LogLevel level);
    static std::string getCurrentTimeString();
};

} // namespace Debug
} // namespace Engine

#define LOG_DEBUG(msg) ::Engine::Debug::Logger::log(::Engine::Debug::LogLevel::Debug, msg, __FILE__, __LINE__)
#define LOG_INFO(msg)  ::Engine::Debug::Logger::log(::Engine::Debug::LogLevel::Info, msg)
#define LOG_WARN(msg)  ::Engine::Debug::Logger::log(::Engine::Debug::LogLevel::Warning, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) ::Engine::Debug::Logger::log(::Engine::Debug::LogLevel::Error, msg, __FILE__, __LINE__)

#endif // DEBUG_LOGGER_HPP
