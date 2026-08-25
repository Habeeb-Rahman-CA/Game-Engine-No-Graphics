#ifndef SYSTEM_GAMEPLAYSYSTEM_HPP
#define SYSTEM_GAMEPLAYSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class GameplaySystem : public ISystem {
public:
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "GameplaySystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_GAMEPLAYSYSTEM_HPP
