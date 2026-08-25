#ifndef SYSTEM_PHYSICSSYSTEM_HPP
#define SYSTEM_PHYSICSSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class PhysicsSystem : public ISystem {
public:
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "PhysicsSystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_PHYSICSSYSTEM_HPP
