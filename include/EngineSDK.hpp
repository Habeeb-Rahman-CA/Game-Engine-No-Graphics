#ifndef ENGINE_SDK_HPP
#define ENGINE_SDK_HPP

// Single Public Header for Game Developers

#include "Core/Engine.hpp"
#include "Core/Time.hpp"
#include "Core/Input.hpp"
#include "World/World.hpp"
#include "Entity/Components.hpp"
#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/Geometry.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Color.hpp"
#include "Renderer/GPU2DRenderer.hpp"
#include "Platform/Platform.hpp"
#include "Platform/Window.hpp"
#include "Platform/FileSystem.hpp"
#include "Audio/Audio.hpp"
#include "Event/EventBus.hpp"
#include "Debug/Logger.hpp"
#include "Scene/SceneSerializer.hpp"

#include <functional>
#include <string>
#include <memory>

namespace Engine {
namespace SDK {

using World = Engine::WorldSystem::World;
using Entity = Engine::WorldSystem::Entity;
using Transform = Engine::EntitySystem::Transform;
using Health = Engine::EntitySystem::Health;
using Velocity = Engine::EntitySystem::Velocity;
using RigidBody2D = Engine::EntitySystem::RigidBody2D;
using EnemyAI = Engine::EntitySystem::EnemyAI;
using AIState = Engine::EntitySystem::AIState;

class EntityHandle {
public:
    EntityHandle(World& world, Entity id) : m_world(&world), m_id(id) {}

    Entity id() const { return m_id; }

    EntityHandle& add_transform(const Math::Vec2& position = Math::Vec2(0, 0), double rotation = 0.0, const Math::Vec2& scale = Math::Vec2(1, 1)) {
        Transform t;
        t.position = Math::Vec3(position.x, position.y, 0.0);
        t.rotation = Math::Vec3(0, 0, rotation);
        t.scale = Math::Vec3(scale.x, scale.y, 1.0);
        m_world->add_transform(m_id, t);
        return *this;
    }

    EntityHandle& add_health(int maxHealth = 100) {
        Health h(maxHealth);
        m_world->add_health(m_id, h);
        return *this;
    }

    EntityHandle& add_rigidbody(double drag = 0.1, bool useGravity = false) {
        RigidBody2D rb;
        rb.drag = drag;
        rb.useGravity = useGravity;
        m_world->add_rigidbody(m_id, rb);
        return *this;
    }

    EntityHandle& add_velocity(const Math::Vec2& vel = Math::Vec2(0, 0)) {
        Velocity v(Math::Vec3(vel.x, vel.y, 0.0));
        m_world->add_velocity(m_id, v);
        return *this;
    }

    EntityHandle& add_ai(AIState state = AIState::Patrol, double detectionRange = 100.0, double attackRange = 30.0) {
        EnemyAI ai;
        ai.state = state;
        ai.detectionRange = detectionRange;
        ai.attackRange = attackRange;
        m_world->add_ai(m_id, ai);
        return *this;
    }

private:
    World* m_world;
    Entity m_id;
};

class GameEngine {
public:
    GameEngine() = default;
    ~GameEngine() = default;

    bool create_window(const std::string& title = "Dungeon Game Engine", uint32_t width = 800, uint32_t height = 600) {
        Platform::WindowProps props(title, width, height);
        return Platform::Platform::getInstance().initialize(props);
    }

    World& world() { return m_world; }

    EntityHandle create_entity(const std::string& name = "Entity") {
        Entity eId = m_world.create_entity(name);
        return EntityHandle(m_world, eId);
    }

    void run(const std::function<void(double dt)>& userUpdateCallback = nullptr) {
        LOG_INFO("Game Engine Main Loop Started via Public SDK.");
        m_running = true;

        while (m_running) {
            auto window = Platform::Platform::getInstance().getWindow();
            if (!window || window->should_close()) break;

            double dt = 0.016; // 60 FPS tick
            Platform::Platform::getInstance().update();

            if (userUpdateCallback) {
                userUpdateCallback(dt);
            }

            if (m_running) {
                m_world.update(dt);
            }
        }

        LOG_INFO("Game Engine Main Loop Stopped.");
    }

    void shutdown() {
        m_running = false;
        Platform::Platform::getInstance().shutdown();
    }

private:
    World m_world;
    bool m_running = false;
};

} // namespace SDK
} // namespace Engine

#endif // ENGINE_SDK_HPP
