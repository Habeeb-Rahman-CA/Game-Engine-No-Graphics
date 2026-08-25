#ifndef CORE_ENGINE_STATE_HPP
#define CORE_ENGINE_STATE_HPP

#include <string>

namespace Engine {
namespace Core {

enum class EngineState {
    Uninitialized,
    Initializing,
    Running,
    Paused,
    Shutdown
};

inline std::string engineStateToString(EngineState state) {
    switch (state) {
        case EngineState::Uninitialized: return "Uninitialized";
        case EngineState::Initializing:  return "Initializing";
        case EngineState::Running:       return "Running";
        case EngineState::Paused:        return "Paused";
        case EngineState::Shutdown:      return "Shutdown";
    }
    return "Unknown";
}

} // namespace Core
} // namespace Engine

#endif // CORE_ENGINE_STATE_HPP
