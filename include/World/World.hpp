#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "Entity/Entity.hpp"
#include "Entity/Components.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

namespace Engine {
namespace WorldSystem {

using Entity = EntitySystem::Entity;
using Transform = EntitySystem::Transform;
using Velocity = EntitySystem::Velocity;
using Health = EntitySystem::Health;
using NameTag = EntitySystem::NameTag;

class World {
public:
    World();
    ~World() = default;

    // Entity Management
    Entity create_entity(const std::string& name = "Entity");
    void destroy_entity(Entity entity);
    size_t get_entity_count() const { return m_entities.size(); }
    const std::vector<Entity>& get_entities() const { return m_entities; }

    // Component Management
    void add_transform(Entity e, const Transform& t);
    void add_velocity(Entity e, const Velocity& v);
    void add_health(Entity e, const Health& h);

    Transform* get_transform(Entity e);
    Velocity* get_velocity(Entity e);
    Health* get_health(Entity e);
    std::string get_name(Entity e) const;

    bool has_transform(Entity e) const;
    bool has_velocity(Entity e) const;
    bool has_health(Entity e) const;

    // World Systems
    void update(double dt);
    void print_world_state() const;

private:
    Entity m_nextEntityId;
    std::vector<Entity> m_entities;

    // Simple Component Storage
    std::unordered_map<Entity, Transform> m_transforms;
    std::unordered_map<Entity, Velocity> m_velocities;
    std::unordered_map<Entity, Health> m_healths;
    std::unordered_map<Entity, NameTag> m_names;
};

} // namespace WorldSystem
} // namespace Engine

#endif // WORLD_WORLD_HPP
