#ifndef ENTITY_COMPONENTS_HPP
#define ENTITY_COMPONENTS_HPP

#include <string>

namespace Engine {
namespace EntitySystem {

struct Transform {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double rotX = 0.0;
    double rotY = 0.0;
    double rotZ = 0.0;

    Transform(double px = 0.0, double py = 0.0, double pz = 0.0)
        : x(px), y(py), z(pz) {}
};

struct Velocity {
    double vx = 0.0;
    double vy = 0.0;
    double vz = 0.0;

    Velocity(double velX = 0.0, double velY = 0.0, double velZ = 0.0)
        : vx(velX), vy(velY), vz(velZ) {}
};

struct Health {
    float current = 100.0f;
    float max = 100.0f;

    Health(float hp = 100.0f, float maxHp = 100.0f)
        : current(hp), max(maxHp) {}

    bool isAlive() const { return current > 0.0f; }
    float getPercentage() const { return max > 0.0f ? (current / max) * 100.0f : 0.0f; }
};

struct NameTag {
    std::string name = "Entity";

    NameTag(const std::string& n = "Entity") : name(n) {}
};

} // namespace EntitySystem
} // namespace Engine

#endif // ENTITY_COMPONENTS_HPP
