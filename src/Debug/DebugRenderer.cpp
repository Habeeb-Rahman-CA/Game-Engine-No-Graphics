#include "Debug/DebugRenderer.hpp"
#include "Renderer/Renderer.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include <iostream>

namespace Engine {
namespace DebugSystem {

DebugFlags DebugRenderer::s_flags;

void DebugRenderer::init() {
    s_flags = DebugFlags();
    LOG_INFO("Debug Renderer Initialized (F1: Colliders | F2: IDs | F3: FPS | F4: Profiler | F5: Physics)");
}

void DebugRenderer::handleInput() {
    using namespace InputSystem;

    if (Input::is_key_pressed(Key::F1)) {
        toggleCollisionBoxes();
        LOG_INFO("[DEBUG TOGGLE] Collision Boxes (F1): " + std::string(s_flags.showCollisionBoxes ? "ON" : "OFF"));
    }
    if (Input::is_key_pressed(Key::F2)) {
        toggleEntityIds();
        LOG_INFO("[DEBUG TOGGLE] Entity IDs & Names (F2): " + std::string(s_flags.showEntityIds ? "ON" : "OFF"));
    }
    if (Input::is_key_pressed(Key::F3)) {
        toggleFPS();
        LOG_INFO("[DEBUG TOGGLE] FPS Display (F3): " + std::string(s_flags.showFPS ? "ON" : "OFF"));
    }
    if (Input::is_key_pressed(Key::F4)) {
        toggleProfiler();
        LOG_INFO("[DEBUG TOGGLE] Profiler Timings (F4): " + std::string(s_flags.showProfiler ? "ON" : "OFF"));
    }
    if (Input::is_key_pressed(Key::F5)) {
        togglePhysicsVectors();
        LOG_INFO("[DEBUG TOGGLE] Physics Vectors (F5): " + std::string(s_flags.showPhysicsVectors ? "ON" : "OFF"));
    }
}

void DebugRenderer::renderDebugOverlay(WorldSystem::World& world, double fps) {
    using namespace RenderSystem;

    // F1: Collision Boxes Wireframes
    if (s_flags.showCollisionBoxes) {
        for (EntitySystem::Entity e : world.get_entities()) {
            auto* transform = world.get_transform(e);
            auto* collider  = world.get_collider(e);

            if (!transform || !collider) continue;
            Math::Vec2 pos(transform->position.x, transform->position.y);

            if (collider->type == EntitySystem::ColliderType::Box) {
                Math::AABB worldBox(collider->box.minBound + pos, collider->box.maxBound + pos);
                Renderer::draw_rect(worldBox, Color::Green, false); // Wireframe outline
            } else if (collider->type == EntitySystem::ColliderType::Circle) {
                Math::Circle worldCircle(collider->circle.center + pos, collider->circle.radius);
                Renderer::draw_circle(worldCircle, Color::Green, false);
            }
        }
    }

    // F5: Physics Vectors (Velocity Lines)
    if (s_flags.showPhysicsVectors) {
        for (EntitySystem::Entity e : world.get_entities()) {
            auto* transform = world.get_transform(e);
            auto* velocity  = world.get_velocity(e);

            if (!transform || !velocity) continue;
            Math::Vec2 start(transform->position.x, transform->position.y);
            Math::Vec2 end = start + Math::Vec2(velocity->value.x, velocity->value.y);
            Renderer::draw_line(start, end, Color::Cyan);
        }
    }

    // F2: Entity IDs & Names
    if (s_flags.showEntityIds) {
        LOG_INFO("--- [F2 DEBUG] ACTIVE ENTITIES ---");
        for (EntitySystem::Entity e : world.get_entities()) {
            auto* t = world.get_transform(e);
            std::string posStr = t ? t->position.toString() : "(N/A)";
            LOG_INFO(" Entity ID: " + std::to_string(e) + " | Name: '" + world.get_name(e) + "' | Position: " + posStr);
        }
    }

    // F3: FPS Counter
    if (s_flags.showFPS) {
        double frameTimeMs = (fps > 0.0) ? (1000.0 / fps) : 0.0;
        LOG_INFO("--- [F3 DEBUG] PERF STATS | FPS: " + std::to_string(fps) + " | Frame Time: " + std::to_string(frameTimeMs) + " ms ---");
    }

    // F4: Profiler Breakdown
    if (s_flags.showProfiler) {
        LOG_INFO("--- [F4 DEBUG] PROFILER SCOPE BREAKDOWN ---");
        Engine::Debug::Profiler::getInstance().printFrameReport();
    }
}

} // namespace DebugSystem
} // namespace Engine
