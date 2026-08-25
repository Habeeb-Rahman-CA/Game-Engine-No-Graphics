#ifndef SYSTEM_MOVEMENTSYSTEM_HPP
#define SYSTEM_MOVEMENTSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class MovementSystem : public ISystem {
public:
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "MovementSystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_MOVEMENTSYSTEM_HPP
