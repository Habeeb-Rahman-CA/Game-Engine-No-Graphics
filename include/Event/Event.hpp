#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include "Entity/Entity.hpp"
#include "Math/Vec3.hpp"
#include <string>
#include <typeindex>

namespace Engine {
namespace EventSystem {

class IEvent {
public:
    virtual ~IEvent() = default;
    virtual std::string getName() const = 0;
};

// 1. Keyboard Event
struct KeyboardEvent : public IEvent {
    char key;
    bool isPressed;

    KeyboardEvent(char k, bool pressed) : key(k), isPressed(pressed) {}
    std::string getName() const override { return "KeyboardEvent"; }
};

// 2. Collision Event
struct CollisionEvent : public IEvent {
    EntitySystem::Entity entityA;
    EntitySystem::Entity entityB;
    Math::Vec3 hitPoint;

    CollisionEvent(EntitySystem::Entity a, EntitySystem::Entity b, Math::Vec3 hit)
        : entityA(a), entityB(b), hitPoint(hit) {}
    std::string getName() const override { return "CollisionEvent"; }
};

// 3. Entity Created Event
struct EntityCreatedEvent : public IEvent {
    EntitySystem::Entity entityId;
    std::string entityName;

    EntityCreatedEvent(EntitySystem::Entity id, const std::string& name)
        : entityId(id), entityName(name) {}
    std::string getName() const override { return "EntityCreatedEvent"; }
};

// 4. Entity Destroyed Event
struct EntityDestroyedEvent : public IEvent {
    EntitySystem::Entity entityId;

    explicit EntityDestroyedEvent(EntitySystem::Entity id) : entityId(id) {}
    std::string getName() const override { return "EntityDestroyedEvent"; }
};

// 5. Game Over Event
struct GameOverEvent : public IEvent {
    std::string reason;
    int finalScore;

    GameOverEvent(const std::string& r, int score) : reason(r), finalScore(score) {}
    std::string getName() const override { return "GameOverEvent"; }
};

} // namespace EventSystem
} // namespace Engine

#endif // EVENT_EVENT_HPP
