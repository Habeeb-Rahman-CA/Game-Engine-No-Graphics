#include "System/GameplaySystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void GameplaySystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("Gameplay");
    (void)dt;

    // Check health states of all entities with Health components
    for (EntitySystem::Entity e : world.get_entities()) {
        EntitySystem::Health* health = world.get_health(e);

        if (health && !health->isAlive()) {
            LOG_INFO("GameplaySystem: Entity [" + std::to_string(e) + "] ('" + 
                     world.get_name(e) + "') has been defeated (0 HP)!");
        }
    }
}

} // namespace System
} // namespace Engine
