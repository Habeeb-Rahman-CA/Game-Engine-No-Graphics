#include "System/MovementSystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void MovementSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("Movement");
    // Process all entities containing both Transform and Velocity components
    for (EntitySystem::Entity e : world.get_entities()) {
        EntitySystem::Transform* transform = world.get_transform(e);
        EntitySystem::Velocity* velocity = world.get_velocity(e);

        if (transform && velocity) {
            transform->position += velocity->value * dt;
        }
    }
}

} // namespace System
} // namespace Engine
