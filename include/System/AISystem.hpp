#ifndef SYSTEM_AISYSTEM_HPP
#define SYSTEM_AISYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class AISystem : public ISystem {
public:
    AISystem() = default;
    ~AISystem() override = default;

    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "AISystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_AISYSTEM_HPP
