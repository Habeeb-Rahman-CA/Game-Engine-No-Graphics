#include "Physics/Collision.hpp"

namespace Engine {
namespace Physics {

// 1. AABB vs AABB
bool Collision::intersects(const Math::AABB& a, const Math::AABB& b) {
    return a.intersects(b);
}

// 2. Circle vs Circle
bool Collision::intersects(const Math::Circle& a, const Math::Circle& b) {
    return a.intersects(b);
}

// 3. Circle vs AABB
bool Collision::intersects(const Math::Circle& circle, const Math::AABB& box) {
    // Find closest point on AABB to Circle center
    double closestX = Math::clamp(circle.center.x, box.minBound.x, box.maxBound.x);
    double closestY = Math::clamp(circle.center.y, box.minBound.y, box.maxBound.y);
    Math::Vec2 closestPoint(closestX, closestY);

    return circle.contains(closestPoint);
}

// 4. Point vs AABB
bool Collision::intersects(const Math::Vec2& point, const Math::AABB& box) {
    return box.contains(point);
}

// Unified Component Collision Resolver
bool Collision::checkCollision(
    const EntitySystem::Collider2D& colA, const Math::Vec2& posA,
    const EntitySystem::Collider2D& colB, const Math::Vec2& posB,
    Math::Vec3& outHitPoint
) {
    using namespace EntitySystem;
    using namespace Math;

    if (colA.type == ColliderType::Box && colB.type == ColliderType::Box) {
        AABB boxA(colA.box.minBound + posA, colA.box.maxBound + posA);
        AABB boxB(colB.box.minBound + posB, colB.box.maxBound + posB);
        if (intersects(boxA, boxB)) {
            outHitPoint = Vec3((posA.x + posB.x) * 0.5, (posA.y + posB.y) * 0.5, 0.0);
            return true;
        }
    } else if (colA.type == ColliderType::Circle && colB.type == ColliderType::Circle) {
        Circle circleA(colA.circle.center + posA, colA.circle.radius);
        Circle circleB(colB.circle.center + posB, colB.circle.radius);
        if (intersects(circleA, circleB)) {
            outHitPoint = Vec3((posA.x + posB.x) * 0.5, (posA.y + posB.y) * 0.5, 0.0);
            return true;
        }
    } else if (colA.type == ColliderType::Circle && colB.type == ColliderType::Box) {
        Circle circleA(colA.circle.center + posA, colA.circle.radius);
        AABB boxB(colB.box.minBound + posB, colB.box.maxBound + posB);
        if (intersects(circleA, boxB)) {
            outHitPoint = Vec3(circleA.center.x, circleA.center.y, 0.0);
            return true;
        }
    } else if (colA.type == ColliderType::Box && colB.type == ColliderType::Circle) {
        AABB boxA(colA.box.minBound + posA, colA.box.maxBound + posA);
        Circle circleB(colB.circle.center + posB, colB.circle.radius);
        if (intersects(circleB, boxA)) {
            outHitPoint = Vec3(circleB.center.x, circleB.center.y, 0.0);
            return true;
        }
    } else if (colA.type == ColliderType::Point && colB.type == ColliderType::Box) {
        Vec2 ptA = colA.point + posA;
        AABB boxB(colB.box.minBound + posB, colB.box.maxBound + posB);
        if (intersects(ptA, boxB)) {
            outHitPoint = Vec3(ptA.x, ptA.y, 0.0);
            return true;
        }
    } else if (colA.type == ColliderType::Box && colB.type == ColliderType::Point) {
        AABB boxA(colA.box.minBound + posA, colA.box.maxBound + posA);
        Vec2 ptB = colB.point + posB;
        if (intersects(ptB, boxA)) {
            outHitPoint = Vec3(ptB.x, ptB.y, 0.0);
            return true;
        }
    }

    return false;
}

} // namespace Physics
} // namespace Engine
