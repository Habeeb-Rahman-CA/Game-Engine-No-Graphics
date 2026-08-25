#ifndef PHYSICS_COLLISION_HPP
#define PHYSICS_COLLISION_HPP

#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include "Entity/Components.hpp"

namespace Engine {
namespace Physics {

class Collision {
public:
    // Core 2D Shape Intersections requested
    static bool intersects(const Math::AABB& a, const Math::AABB& b);
    static bool intersects(const Math::Circle& a, const Math::Circle& b);
    static bool intersects(const Math::Circle& circle, const Math::AABB& box);
    static bool intersects(const Math::Vec2& point, const Math::AABB& box);

    // Unified Component Collision Resolver
    static bool checkCollision(
        const EntitySystem::Collider2D& colA, const Math::Vec2& posA,
        const EntitySystem::Collider2D& colB, const Math::Vec2& posB,
        Math::Vec3& outHitPoint
    );
};

} // namespace Physics
} // namespace Engine

#endif // PHYSICS_COLLISION_HPP
