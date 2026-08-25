#include <EngineSDK.hpp>

int main() {
    using namespace Engine::SDK;
    using namespace Engine::Math;
    using namespace Engine::RenderSystem;

    LOG_INFO("==========================================================================");
    LOG_INFO("              DUNGEON GAME - PUBLIC ENGINE SDK DEMO                        ");
    LOG_INFO("==========================================================================");

    // Initialize Engine via Public API
    GameEngine engine;
    if (!engine.create_window("Dungeon Quest 2D - Public SDK", 800, 600)) {
        LOG_ERROR("Failed to create engine window!");
        return -1;
    }

    // Access World & Create Entities via Public Fluent SDK API
    World& world = engine.world();
    (void)world; // Accessed directly via engine wrapper or world reference

    auto player = engine.create_entity("Hero")
        .add_transform(Vec2(100, 250))
        .add_health(100)
        .add_velocity(Vec2(50, 0))
        .add_rigidbody(0.1, false);

    auto enemy = engine.create_entity("Skeleton_Warrior")
        .add_transform(Vec2(500, 250))
        .add_health(60)
        .add_velocity(Vec2(-20, 0))
        .add_ai(AIState::Chase, 200.0, 40.0);

    LOG_INFO("Dungeon entities initialized via Public Engine SDK:");
    LOG_INFO("  Created Player entity ID: " + std::to_string(player.id()));
    LOG_INFO("  Created Enemy entity ID:  " + std::to_string(enemy.id()));

    // Set GPU Renderer Backend
    auto window = Engine::Platform::Platform::getInstance().getWindow();
    Renderer::setBackend(std::make_unique<GPU2DRenderer>(window));

    // Custom Game Loop Callback
    int frameCount = 0;
    engine.run([&](double dt) {
        (void)dt;
        frameCount++;

        Renderer::begin_frame();
        Renderer::clear(Color(0.1f, 0.05f, 0.15f, 1.0f)); // Dark Dungeon BG

        // Draw Player & Enemy Sprites / Shapes
        Renderer::draw_rect(AABB(Vec2(80, 220), Vec2(120, 280)), Color::Cyan, true);
        Renderer::draw_circle(Circle(Vec2(500, 250), 30), Color::Red, true);
        Renderer::draw_text("DUNGEON QUEST 2D (PUBLIC SDK)", Vec2(220, 40), Color::Yellow, 1.5f);

        Renderer::end_frame();

        if (frameCount >= 5) {
            LOG_INFO("Dungeon Game loop execution completed 5 frames successfully via Public Engine SDK!");
            engine.shutdown();
        }
    });

    LOG_INFO("==========================================================================");
    return 0;
}
