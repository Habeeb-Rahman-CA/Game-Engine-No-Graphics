#include "World/World.hpp"
#include "Debug/Logger.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace WorldSystem {

World::World() : m_nextEntityId(1) {}

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
        m_healths.erase(entity);
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

void World::add_health(Entity e, const Health& h) {
    m_healths[e] = h;
}

Transform* World::get_transform(Entity e) {
    auto it = m_transforms.find(e);
    return (it != m_transforms.end()) ? &it->second : nullptr;
}

Velocity* World::get_velocity(Entity e) {
    auto it = m_velocities.find(e);
    return (it != m_velocities.end()) ? &it->second : nullptr;
}

Health* World::get_health(Entity e) {
    auto it = m_healths.find(e);
    return (it != m_healths.end()) ? &it->second : nullptr;
}

std::string World::get_name(Entity e) const {
    auto it = m_names.find(e);
    return (it != m_names.end()) ? it->second.name : "Unknown";
}

bool World::has_transform(Entity e) const {
    return m_transforms.find(e) != m_transforms.end();
}

bool World::has_velocity(Entity e) const {
    return m_velocities.find(e) != m_velocities.end();
}

bool World::has_health(Entity e) const {
    return m_healths.find(e) != m_healths.end();
}

void World::update(double dt) {
    // Movement System: Update position for entities with both Transform and Velocity
    for (Entity e : m_entities) {
        Transform* transform = get_transform(e);
        Velocity* velocity = get_velocity(e);

        if (transform && velocity) {
            transform->x += velocity->vx * dt;
            transform->y += velocity->vy * dt;
            transform->z += velocity->vz * dt;
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
            ss << std::fixed << std::setprecision(2);
            ss << " | Transform(pos: " << tIt->second.x << ", " << tIt->second.y << ", " << tIt->second.z << ")";
        }

        auto vIt = m_velocities.find(e);
        if (vIt != m_velocities.end()) {
            ss << std::fixed << std::setprecision(2);
            ss << " | Velocity(vel: " << vIt->second.vx << ", " << vIt->second.vy << ", " << vIt->second.vz << ")";
        }

        auto hIt = m_healths.find(e);
        if (hIt != m_healths.end()) {
            ss << " | Health(" << hIt->second.current << "/" << hIt->second.max << " HP)";
        }

        LOG_INFO(ss.str());
    }
    LOG_INFO("-----------------------------------------------------");
}

} // namespace WorldSystem
} // namespace Engine
