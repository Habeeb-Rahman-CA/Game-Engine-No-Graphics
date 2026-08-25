#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include "Math/Vec3.hpp"
#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include <string>

namespace Engine {
namespace EntitySystem {

using Vec3 = Math::Vec3;
using Vec2 = Math::Vec2;

struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    Transform(Vec3 pos = Vec3(0.0, 0.0, 0.0), 
              Vec3 rot = Vec3(0.0, 0.0, 0.0), 
              Vec3 scl = Vec3(1.0, 1.0, 1.0))
        : position(pos), rotation(rot), scale(scl) {}
};

struct Velocity {
    Vec3 value;

    Velocity(Vec3 vel = Vec3(0.0, 0.0, 0.0)) : value(vel) {}
};

struct Acceleration {
    Vec3 value;

    Acceleration(Vec3 acc = Vec3(0.0, 0.0, 0.0)) : value(acc) {}
};

struct RigidBody2D {
    bool useGravity = true;
    double gravityScale = 1.0;
    double drag = 0.05;         // Linear air resistance damping
    double restitution = 0.75;  // Bounciness elasticity (0 = no bounce, 1 = perfectly elastic)
    bool isGrounded = false;

    RigidBody2D(bool gravity = true, double scale = 1.0, double d = 0.05, double rest = 0.75)
        : useGravity(gravity), gravityScale(scale), drag(d), restitution(rest), isGrounded(false) {}
};

struct Health {
    int value = 100;

    Health(int hp = 100) : value(hp) {}
    bool isAlive() const { return value > 0; }
};

struct NameTag {
    std::string name = "Entity";

    NameTag(const std::string& n = "Entity") : name(n) {}
};

enum class ColliderType {
    Box,
    Circle,
    Point
};

struct Collider2D {
    ColliderType type = ColliderType::Box;
    Math::AABB box;
    Math::Circle circle;
    Math::Vec2 point;

    static Collider2D MakeBox(const Math::Vec2& size) {
        Collider2D c;
        c.type = ColliderType::Box;
        c.box = Math::AABB::fromCenterSize(Math::Vec2(0.0, 0.0), size);
        return c;
    }

    static Collider2D MakeCircle(double radius) {
        Collider2D c;
        c.type = ColliderType::Circle;
        c.circle = Math::Circle(Math::Vec2(0.0, 0.0), radius);
        return c;
    }

    static Collider2D MakePoint(const Math::Vec2& pt = Math::Vec2(0.0, 0.0)) {
        Collider2D c;
        c.type = ColliderType::Point;
        c.point = pt;
        return c;
    }
};

} // namespace EntitySystem
} // namespace Engine

#endif // ENTITY_COMPONENTS_HPP
