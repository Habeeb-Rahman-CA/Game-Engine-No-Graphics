#ifndef SYSTEM_ANIMATIONSYSTEM_HPP
#define SYSTEM_ANIMATIONSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class AnimationSystem : public ISystem {
public:
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "AnimationSystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_ANIMATIONSYSTEM_HPP
