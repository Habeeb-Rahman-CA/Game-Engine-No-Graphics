#include "System/PhysicsSystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace System {

void PhysicsSystem::update(WorldSystem::World& world, double dt) {
    (void)dt; // Unused for instant collision checks

    // Check collision between moving entities and walls
    for (EntitySystem::Entity e : world.get_entities()) {
        EntitySystem::Transform* transform = world.get_transform(e);
        EntitySystem::Velocity* velocity = world.get_velocity(e);

        if (transform && velocity) {
            // Simple boundary check at x >= 100.0 (Wall location)
            if (transform->position.x >= 100.0) {
                LOG_WARN("PhysicsSystem: Entity [" + std::to_string(e) + "] ('" + 
                         world.get_name(e) + "') collided with Wall boundary at x=100.0! Resetting velocity.");
                velocity->value = Engine::Math::Vec3(0.0, 0.0, 0.0);
                transform->position.x = 100.0;
            }
        }
    }
}

} // namespace System
} // namespace Engine
