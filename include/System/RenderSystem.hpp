#ifndef SYSTEM_RENDERSYSTEM_HPP
#define SYSTEM_RENDERSYSTEM_HPP

#include "System/ISystem.hpp"

namespace Engine {
namespace System {

class RenderSystem : public ISystem {
public:
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "RenderSystem"; }
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_RENDERSYSTEM_HPP
