#ifndef DEBUG_DEBUGRENDERER_HPP
#define DEBUG_DEBUGRENDERER_HPP

#include "Input/Input.hpp"
#include "World/World.hpp"

namespace Engine {
namespace DebugSystem {

struct DebugFlags {
    bool showCollisionBoxes = false; // F1
    bool showEntityIds = false;      // F2
    bool showFPS = false;            // F3
    bool showProfiler = false;       // F4
    bool showPhysicsVectors = false;  // F5
};

class DebugRenderer {
public:
    static void init();
    static void handleInput();
    static void renderDebugOverlay(WorldSystem::World& world, double fps);

    static DebugFlags& getFlags() { return s_flags; }
    static void toggleCollisionBoxes() { s_flags.showCollisionBoxes = !s_flags.showCollisionBoxes; }
    static void toggleEntityIds() { s_flags.showEntityIds = !s_flags.showEntityIds; }
    static void toggleFPS() { s_flags.showFPS = !s_flags.showFPS; }
    static void toggleProfiler() { s_flags.showProfiler = !s_flags.showProfiler; }
    static void togglePhysicsVectors() { s_flags.showPhysicsVectors = !s_flags.showPhysicsVectors; }

private:
    static DebugFlags s_flags;
};

} // namespace DebugSystem
} // namespace Engine

#endif // DEBUG_DEBUGRENDERER_HPP
