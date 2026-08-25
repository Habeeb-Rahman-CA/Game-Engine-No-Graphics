#ifndef WORLD_WORLD_HPP
#define WORLD_WORLD_HPP

#include "Entity/Entity.hpp"
#include "Entity/Components.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "System/ISystem.hpp"

namespace Engine {
namespace WorldSystem {

using Entity = EntitySystem::Entity;
using Transform = EntitySystem::Transform;
using Velocity = EntitySystem::Velocity;
using Acceleration = EntitySystem::Acceleration;
using RigidBody2D = EntitySystem::RigidBody2D;
using Health = EntitySystem::Health;
using NameTag = EntitySystem::NameTag;
using Collider2D = EntitySystem::Collider2D;
using ISystem = Engine::System::ISystem;

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
    void add_acceleration(Entity e, const Acceleration& a);
    void add_rigidbody(Entity e, const RigidBody2D& rb);
    void add_health(Entity e, const Health& h);
    void add_collider(Entity e, const Collider2D& c);

    Transform* get_transform(Entity e);
    Velocity* get_velocity(Entity e);
    Acceleration* get_acceleration(Entity e);
    RigidBody2D* get_rigidbody(Entity e);
    Health* get_health(Entity e);
    Collider2D* get_collider(Entity e);
    std::string get_name(Entity e) const;

    bool has_transform(Entity e) const;
    bool has_velocity(Entity e) const;
    bool has_acceleration(Entity e) const;
    bool has_rigidbody(Entity e) const;
    bool has_health(Entity e) const;
    bool has_collider(Entity e) const;

    // System Pipeline Management
    void add_system(std::unique_ptr<ISystem> system);

    // World Systems Execution
    void update(double dt);
    void print_world_state() const;

private:
    Entity m_nextEntityId;
    std::vector<Entity> m_entities;

    // Simple Component Storage
    std::unordered_map<Entity, Transform> m_transforms;
    std::unordered_map<Entity, Velocity> m_velocities;
    std::unordered_map<Entity, Acceleration> m_accelerations;
    std::unordered_map<Entity, RigidBody2D> m_rigidbodies;
    std::unordered_map<Entity, Health> m_healths;
    std::unordered_map<Entity, Collider2D> m_colliders;
    std::unordered_map<Entity, NameTag> m_names;

    // Registered Systems Pipeline
    std::vector<std::unique_ptr<ISystem>> m_systems;
};

} // namespace WorldSystem
} // namespace Engine

#endif // WORLD_WORLD_HPP
