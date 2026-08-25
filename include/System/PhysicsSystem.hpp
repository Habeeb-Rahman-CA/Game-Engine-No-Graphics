#ifndef SYSTEM_PHYSICSSYSTEM_HPP
#define SYSTEM_PHYSICSSYSTEM_HPP

#include "System/ISystem.hpp"
#include "Event/EventBus.hpp"

namespace Engine {
namespace System {

class PhysicsSystem : public ISystem {
public:
    explicit PhysicsSystem(EventSystem::EventBus* eventBus = nullptr) : m_eventBus(eventBus) {}

    void setEventBus(EventSystem::EventBus* eventBus) { m_eventBus = eventBus; }
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "PhysicsSystem"; }

private:
    EventSystem::EventBus* m_eventBus = nullptr;
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_PHYSICSSYSTEM_HPP
