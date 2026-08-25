#include "World/World.hpp"
#include "Scene/SceneSerializer.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace WorldSystem {

World::World() : m_nextEntityId(1) {}

bool World::load_scene(const std::string& filePath) {
    return Engine::SceneSystem::SceneSerializer::load_scene(*this, filePath);
}

bool World::save_scene(const std::string& filePath) {
    return Engine::SceneSystem::SceneSerializer::save_scene(*this, filePath);
}

Entity World::create_entity(const std::string& name) {
    Entity id = m_nextEntityId++;
    m_entities.push_back(id);
    m_names[id] = NameTag(name.empty() ? ("Entity_" + std::to_string(id)) : name);
    LOG_INFO("Created Entity ID: " + std::to_string(id) + " ('" + m_names[id].name + "')");
    return id;
}

void World::destroy_entity(Entity entity) {
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end()) {
        std::string name = get_name(entity);
        m_entities.erase(it);
        m_transforms.erase(entity);
        m_velocities.erase(entity);
        m_accelerations.erase(entity);
        m_rigidbodies.erase(entity);
        m_healths.erase(entity);
        m_colliders.erase(entity);
        m_sprites.erase(entity);
        m_animations.erase(entity);
        m_spritesheets.erase(entity);
        m_animators.erase(entity);
        m_ais.erase(entity);
        m_names.erase(entity);
        LOG_INFO("Destroyed Entity ID: " + std::to_string(entity) + " ('" + name + "')");
    }
}

void World::add_transform(Entity e, const Transform& t) {
    m_transforms[e] = t;
}

void World::add_velocity(Entity e, const Velocity& v) {
    m_velocities[e] = v;
}

void World::add_acceleration(Entity e, const Acceleration& a) {
    m_accelerations[e] = a;
}

void World::add_rigidbody(Entity e, const RigidBody2D& rb) {
    m_rigidbodies[e] = rb;
}

void World::add_health(Entity e, const Health& h) {
    m_healths[e] = h;
}

void World::add_collider(Entity e, const Collider2D& c) {
    m_colliders[e] = c;
}

void World::add_sprite(Entity e, const Sprite& s) {
    m_sprites[e] = s;
}

void World::add_animation(Entity e, const Animation& a) {
    m_animations[e] = a;
}

void World::add_spritesheet(Entity e, const SpriteSheet& ss) {
    m_spritesheets[e] = ss;
}

void World::add_animator(Entity e, const Animator& anim) {
    m_animators[e] = anim;
}

Transform* World::get_transform(Entity e) {
    auto it = m_transforms.find(e);
    return (it != m_transforms.end()) ? &it->second : nullptr;
}

Velocity* World::get_velocity(Entity e) {
    auto it = m_velocities.find(e);
    return (it != m_velocities.end()) ? &it->second : nullptr;
}

Acceleration* World::get_acceleration(Entity e) {
    auto it = m_accelerations.find(e);
    return (it != m_accelerations.end()) ? &it->second : nullptr;
}

RigidBody2D* World::get_rigidbody(Entity e) {
    auto it = m_rigidbodies.find(e);
    return (it != m_rigidbodies.end()) ? &it->second : nullptr;
}

Health* World::get_health(Entity e) {
    auto it = m_healths.find(e);
    return (it != m_healths.end()) ? &it->second : nullptr;
}

Collider2D* World::get_collider(Entity e) {
    auto it = m_colliders.find(e);
    return (it != m_colliders.end()) ? &it->second : nullptr;
}

Sprite* World::get_sprite(Entity e) {
    auto it = m_sprites.find(e);
    return (it != m_sprites.end()) ? &it->second : nullptr;
}

Animation* World::get_animation(Entity e) {
    auto it = m_animations.find(e);
    return (it != m_animations.end()) ? &it->second : nullptr;
}

SpriteSheet* World::get_spritesheet(Entity e) {
    auto it = m_spritesheets.find(e);
    return (it != m_spritesheets.end()) ? &it->second : nullptr;
}

Animator* World::get_animator(Entity e) {
    auto it = m_animators.find(e);
    return (it != m_animators.end()) ? &it->second : nullptr;
}

std::string World::get_name(Entity e) const {
    auto it = m_names.find(e);
    return (it != m_names.end()) ? it->second.name : "Unknown";
}

void World::set_name(Entity e, const std::string& name) {
    m_names[e] = NameTag(name);
}

bool World::has_transform(Entity e) const {
    return m_transforms.find(e) != m_transforms.end();
}

bool World::has_velocity(Entity e) const {
    return m_velocities.find(e) != m_velocities.end();
}

bool World::has_acceleration(Entity e) const {
    return m_accelerations.find(e) != m_accelerations.end();
}

bool World::has_rigidbody(Entity e) const {
    return m_rigidbodies.find(e) != m_rigidbodies.end();
}

bool World::has_health(Entity e) const {
    return m_healths.find(e) != m_healths.end();
}

bool World::has_collider(Entity e) const {
    return m_colliders.find(e) != m_colliders.end();
}

bool World::has_sprite(Entity e) const {
    return m_sprites.find(e) != m_sprites.end();
}

bool World::has_animation(Entity e) const {
    return m_animations.find(e) != m_animations.end();
}

bool World::has_spritesheet(Entity e) const {
    return m_spritesheets.find(e) != m_spritesheets.end();
}

bool World::has_animator(Entity e) const {
    return m_animators.find(e) != m_animators.end();
}

void World::add_ai(Entity e, const EnemyAI& ai) {
    m_ais[e] = ai;
}

EnemyAI* World::get_ai(Entity e) {
    auto it = m_ais.find(e);
    return (it != m_ais.end()) ? &it->second : nullptr;
}

bool World::has_ai(Entity e) const {
    return m_ais.find(e) != m_ais.end();
}

#include "System/ISystem.hpp"

void World::add_system(std::unique_ptr<ISystem> system) {
    LOG_INFO("Registered System: " + system->getName());
    m_systems.push_back(std::move(system));
}

void World::update(double dt) {
    PROFILE_SCOPE("ECS");
    // If specific systems are registered, update them sequentially
    if (!m_systems.empty()) {
        for (auto& system : m_systems) {
            system->update(*this, dt);
        }
    } else {
        // Default Movement System: Update position for entities with both Transform and Velocity
        for (Entity e : m_entities) {
            Transform* transform = get_transform(e);
            Velocity* velocity = get_velocity(e);

            if (transform && velocity) {
                transform->position += velocity->value * dt;
            }
        }
    }
}

void World::print_world_state() const {
    LOG_INFO("---------------- WORLD STATE SUMMARY ----------------");
    for (Entity e : m_entities) {
        std::stringstream ss;
        ss << "Entity [" << e << "] (" << get_name(e) << "):";

        auto tIt = m_transforms.find(e);
        if (tIt != m_transforms.end()) {
            ss << " | Transform(pos: " << tIt->second.position.toString()
               << ", rot: " << tIt->second.rotation.toString()
               << ", scale: " << tIt->second.scale.toString() << ")";
        }

        auto vIt = m_velocities.find(e);
        if (vIt != m_velocities.end()) {
            ss << " | Velocity(" << vIt->second.value.toString() << ")";
        }

        auto hIt = m_healths.find(e);
        if (hIt != m_healths.end()) {
            ss << " | Health(" << hIt->second.value << " HP)";
        }

        LOG_INFO(ss.str());
    }
    LOG_INFO("-----------------------------------------------------");
}

} // namespace WorldSystem
} // namespace Engine
