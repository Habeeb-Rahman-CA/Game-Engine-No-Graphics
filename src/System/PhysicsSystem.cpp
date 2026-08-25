#include "System/PhysicsSystem.hpp"
#include "World/World.hpp"
#include "Physics/Collision.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void PhysicsSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("Physics");
    (void)dt;

    const auto& entities = world.get_entities();
    size_t count = entities.size();

    // Pairwise 2D collision detection loop
    for (size_t i = 0; i < count; ++i) {
        EntitySystem::Entity eA = entities[i];
        auto* transformA = world.get_transform(eA);
        auto* colliderA  = world.get_collider(eA);

        if (!transformA) continue;

        // 1. Boundary Check (Legacy Wall Collision)
        auto* velocityA = world.get_velocity(eA);
        if (velocityA && transformA->position.x >= 100.0) {
            LOG_WARN("PhysicsSystem: Entity [" + std::to_string(eA) + "] ('" + 
                     world.get_name(eA) + "') collided with Wall boundary at x=100.0!");
            velocityA->value = Math::Vec3(0.0, 0.0, 0.0);
            transformA->position.x = 100.0;
        }

        if (!colliderA) continue;

        for (size_t j = i + 1; j < count; ++j) {
            EntitySystem::Entity eB = entities[j];
            auto* transformB = world.get_transform(eB);
            auto* colliderB  = world.get_collider(eB);

            if (!transformB || !colliderB) continue;

            Math::Vec2 posA(transformA->position.x, transformA->position.y);
            Math::Vec2 posB(transformB->position.x, transformB->position.y);
            Math::Vec3 hitPoint(0, 0, 0);

            if (Physics::Collision::checkCollision(*colliderA, posA, *colliderB, posB, hitPoint)) {
                LOG_WARN("[Collision Detected] Entity [" + std::to_string(eA) + "] ('" + world.get_name(eA) +
                         "') <---> Entity [" + std::to_string(eB) + "] ('" + world.get_name(eB) +
                         "') at (" + std::to_string(hitPoint.x) + ", " + std::to_string(hitPoint.y) + ")");

                if (m_eventBus) {
                    m_eventBus->publish(EventSystem::CollisionEvent(eA, eB, hitPoint));
                }
            }
        }
    }
}

} // namespace System
} // namespace Engine
