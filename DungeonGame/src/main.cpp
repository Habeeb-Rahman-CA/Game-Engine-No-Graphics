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

    // Animated Game Loop (runs until window is closed)
    float timeAcc = 0.0f;
    engine.run([&](double dt) {
        timeAcc += static_cast<float>(dt);

        // Animate positions for active gameplay feedback
        float playerY = 250.0f + std::sin(timeAcc * 3.0f) * 40.0f;
        float enemyX  = 500.0f + std::cos(timeAcc * 2.0f) * 60.0f;

        Renderer::begin_frame();
        Renderer::clear(Color(0.08f, 0.05f, 0.12f, 1.0f)); // Dark Dungeon BG

        // Draw Player (Cyan Box) & Enemy (Red Pulsing Circle)
        Renderer::draw_rect(AABB(Vec2(80, playerY - 30), Vec2(140, playerY + 30)), Color::Cyan, true);
        Renderer::draw_circle(Circle(Vec2(enemyX, 250), 35.0f + std::sin(timeAcc * 4.0f) * 5.0f), Color::Red, true);
        Renderer::draw_line(Vec2(100, 500), Vec2(700, 500), Color::Green);

        // HUD Text
        Renderer::draw_text("DUNGEON QUEST 2D - PUBLIC SDK DEMO", Vec2(180, 40), Color::Yellow, 1.5f);
        Renderer::draw_text("Press Window Close [X] to Exit Game", Vec2(220, 540), Color::White, 1.0f);

        Renderer::end_frame();
    });

    LOG_INFO("==========================================================================");
    return 0;
}
