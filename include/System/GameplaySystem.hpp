#ifndef SYSTEM_GAMEPLAYSYSTEM_HPP
#define SYSTEM_GAMEPLAYSYSTEM_HPP

#include "System/ISystem.hpp"
#include "Event/EventBus.hpp"

namespace Engine {
namespace System {

class GameplaySystem : public ISystem {
public:
    explicit GameplaySystem(EventSystem::EventBus* eventBus = nullptr);

    void setEventBus(EventSystem::EventBus* eventBus);
    void setWorld(WorldSystem::World* world) { m_currentWorld = world; }
    void update(WorldSystem::World& world, double dt) override;
    std::string getName() const override { return "GameplaySystem"; }

    void onCollision(const EventSystem::CollisionEvent& event);

private:
    EventSystem::EventBus* m_eventBus = nullptr;
    WorldSystem::World* m_currentWorld = nullptr;
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_GAMEPLAYSYSTEM_HPP
