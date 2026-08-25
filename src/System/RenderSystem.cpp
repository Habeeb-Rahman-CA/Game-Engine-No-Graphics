#include "System/RenderSystem.hpp"
#include "World/World.hpp"
#include "Renderer/Renderer.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"

namespace Engine {
namespace System {

void RenderSystem::update(WorldSystem::World& world, double dt) {
    PROFILE_SCOPE("RenderSystem");
    (void)dt;

    using namespace RenderSystem;

    Renderer::begin_frame();
    Renderer::clear(Color::Black);

    for (EntitySystem::Entity e : world.get_entities()) {
        auto* transform = world.get_transform(e);
        auto* collider  = world.get_collider(e);

        if (!transform) continue;

        Math::Vec2 pos(transform->position.x, transform->position.y);

        if (collider) {
            if (collider->type == EntitySystem::ColliderType::Box) {
                Math::AABB worldBox(collider->box.minBound + pos, collider->box.maxBound + pos);
                Renderer::draw_rect(worldBox, Color::Green, true);
            } else if (collider->type == EntitySystem::ColliderType::Circle) {
                Math::Circle worldCircle(collider->circle.center + pos, collider->circle.radius);
                Renderer::draw_circle(worldCircle, Color::Red, true);
            } else if (collider->type == EntitySystem::ColliderType::Point) {
                Math::Vec2 pt = collider->point + pos;
                Renderer::draw_line(pt, pt + Math::Vec2(1, 1), Color::Yellow);
            }
        } else {
            // Default sprite representation for non-collider entities
            Renderer::draw_sprite("default", pos, Math::Vec2(4, 2));
        }
    }

    Renderer::end_frame();
}

} // namespace System
} // namespace Engine
