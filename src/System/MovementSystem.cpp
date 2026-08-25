#include "System/MovementSystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include <cmath>

namespace Engine {
namespace System {

void MovementSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("Movement");

    // Standard Gravity vector
    const Math::Vec3 GRAVITY(0.0, -9.81, 0.0);

    for (EntitySystem::Entity e : world.get_entities()) {
        auto* transform    = world.get_transform(e);
        auto* velocity     = world.get_velocity(e);
        auto* acceleration = world.get_acceleration(e);
        auto* rigidbody    = world.get_rigidbody(e);

        if (!transform || !velocity) continue;

        Math::Vec3 totalAcc(0.0, 0.0, 0.0);

        // 1. Acceleration Accumulation
        if (acceleration) {
            totalAcc += acceleration->value;
        }

        // 2. Gravity Force
        if (rigidbody && rigidbody->useGravity) {
            totalAcc += GRAVITY * rigidbody->gravityScale;
        }

        // 3. Euler Integration: velocity += acceleration * dt
        velocity->value += totalAcc * dt;

        // 4. Drag / Air Resistance Damping
        if (rigidbody && rigidbody->drag > 0.0) {
            velocity->value = velocity->value * (1.0 / (1.0 + rigidbody->drag * dt));
        }

        // 5. Euler Integration: position += velocity * dt
        transform->position += velocity->value * dt;

        // 6. Ground Collision & Bouncing / Grounded Logic (Ground plane at Y = 0.0)
        if (transform->position.y <= 0.0) {
            transform->position.y = 0.0;
            if (rigidbody) {
                rigidbody->isGrounded = true;
                if (rigidbody->restitution > 0.0 && std::abs(velocity->value.y) > 0.5) {
                    // Reverse vertical velocity scaled by restitution (bounciness)
                    velocity->value.y = -velocity->value.y * rigidbody->restitution;
                } else {
                    velocity->value.y = 0.0;
                }
            } else {
                velocity->value.y = 0.0;
            }
        } else if (rigidbody) {
            rigidbody->isGrounded = false;
        }
    }
}

} // namespace System
} // namespace Engine
