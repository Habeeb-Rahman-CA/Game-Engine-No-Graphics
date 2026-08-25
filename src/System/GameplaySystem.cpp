#include "System/GameplaySystem.hpp"
#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

GameplaySystem::GameplaySystem(EventSystem::EventBus* eventBus) {
    setEventBus(eventBus);
}

void GameplaySystem::setEventBus(EventSystem::EventBus* eventBus) {
    m_eventBus = eventBus;
    if (m_eventBus) {
        m_eventBus->subscribe<EventSystem::CollisionEvent>(
            std::bind(&GameplaySystem::onCollision, this, std::placeholders::_1)
        );
    }
}

void GameplaySystem::onCollision(const EventSystem::CollisionEvent& event) {
    std::string nameA = m_currentWorld ? m_currentWorld->get_name(event.entityA) : std::to_string(event.entityA);
    std::string nameB = m_currentWorld ? m_currentWorld->get_name(event.entityB) : std::to_string(event.entityB);

    LOG_INFO("[GameplaySystem Event Response] Collision handled between '" + nameA + "' and '" + nameB + "'!");

    if (m_currentWorld) {
        auto* hpA = m_currentWorld->get_health(event.entityA);
        auto* hpB = m_currentWorld->get_health(event.entityB);

        if (hpA) hpA->value -= 10;
        if (hpB) hpB->value -= 10;
    }
}

void GameplaySystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("Gameplay");
    (void)dt;
    m_currentWorld = &world;

    for (EntitySystem::Entity e : world.get_entities()) {
        EntitySystem::Health* health = world.get_health(e);
        if (health && !health->isAlive()) {
            LOG_INFO("GameplaySystem: Entity [" + std::to_string(e) + "] ('" + 
                     world.get_name(e) + "') has been defeated (0 HP)!");
        }
    }
}

} // namespace System
} // namespace Engine
