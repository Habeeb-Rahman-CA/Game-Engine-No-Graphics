#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include "Math/Vec3.hpp"
#include <string>

namespace Engine {
namespace EntitySystem {

using Vec3 = Math::Vec3;

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

struct Health {
    int value = 100;

    Health(int hp = 100) : value(hp) {}
    bool isAlive() const { return value > 0; }
};

struct NameTag {
    std::string name = "Entity";

    NameTag(const std::string& n = "Entity") : name(n) {}
};

} // namespace EntitySystem
} // namespace Engine

#endif // ENTITY_COMPONENTS_HPP
