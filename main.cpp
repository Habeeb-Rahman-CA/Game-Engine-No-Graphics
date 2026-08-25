#include "Core/Engine.hpp"
#include "Debug/Logger.hpp"
#include "Math/Math.hpp"
#include <iostream>
#include <string>
#include <cstring>

void printHelp(const char* progName) {
    std::cout << "Usage: " << progName << " [options]\n\n"
              << "Options:\n"
              << "  --frames <N>      Run for N frames then cleanly shutdown (0 = infinite) [default: 0]\n"
              << "  --fps <N>         Set target FPS [default: 60]\n"
              << "  --fixed-hz <N>    Set fixed update rate in Hz [default: 60]\n"
              << "  --log-deltas      Log per-frame delta_time values (Phase 2 demo)\n"
              << "  --log-fixed       Log fixed timestep simulation ticks (Phase 3 demo)\n"
              << "  --log-world       Log world entities and component states (Phase 4 demo)\n"
              << "  --phase2          Run Phase 2 Time System demonstration\n"
              << "  --phase3          Run Phase 3 Fixed Timestep demonstration\n"
              << "  --phase4          Run Phase 4 Entity System demonstration\n"
              << "  --phase5          Run Phase 5 Component Archetype demonstration\n"
              << "  --phase6          Run Phase 6 System Pipeline demonstration\n"
              << "  --phase7          Run Phase 7 Memory Allocators & Benchmark\n"
              << "  --phase8          Run Phase 8 Resource Manager & Asset Cache\n"
              << "  --phase9          Run Phase 9 Event System demonstration\n"
              << "  --profiler        Run Phase 10 Instrumentation Profiler\n"
              << "  --math            Run Math Library demonstration\n"
              << "  --no-stats        Disable periodic render statistics printing\n"
              << "  --help, -h        Show this help message\n";
}

void runPhase3Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 3 DEMO: DECOUPLING RENDER FPS FROM FIXED TIMESTEP (60 Hz)   ");
    LOG_INFO("==========================================================================");

    LOG_INFO("\n--- SCENARIO A: FAST RENDERING (120 FPS Render | 60 Hz Fixed Physics) ---");
    {
        Engine::Core::EngineConfig config;
        config.targetFps = 120.0;
        config.fixedDeltaTime = 1.0 / 60.0;
        config.maxFramesToRun = 6;
        config.logFrameDeltas = true;
        config.logFixedUpdates = true;
        config.showStats = false;

        Engine::Core::Engine engine(config);
        engine.initialize();
        engine.run();
    }

    LOG_INFO("\n--- SCENARIO B: SLOW RENDERING (30 FPS Render | 60 Hz Fixed Physics) ---");
    {
        Engine::Core::EngineConfig config;
        config.targetFps = 30.0;
        config.fixedDeltaTime = 1.0 / 60.0;
        config.maxFramesToRun = 4;
        config.logFrameDeltas = true;
        config.logFixedUpdates = true;
        config.showStats = false;

        Engine::Core::Engine engine(config);
        engine.initialize();
        engine.run();
    }
}

void runPhase4Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 4 DEMO: LIGHTWEIGHT ENTITY & COMPONENT SYSTEM                ");
    LOG_INFO("==========================================================================");

    Engine::Core::EngineConfig config;
    config.targetFps = 60.0;
    config.fixedDeltaTime = 1.0 / 60.0;
    config.maxFramesToRun = 3;
    config.showStats = false;

    Engine::Core::Engine engine(config);
    engine.initialize();

    LOG_INFO("\n=== INITIAL WORLD ENTITY STATES ===");
    engine.getWorld().print_world_state();

    LOG_INFO("\n=== RUNNING FIXED MOVEMENT SIMULATION FOR 3 TICKS ===");
    engine.run();

    LOG_INFO("\n=== FINAL WORLD ENTITY STATES ===");
    engine.getWorld().print_world_state();
}

void runPhase5Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 5 DEMO: COMPONENT ARCHETYPES (Player, Bullet, Wall)          ");
    LOG_INFO("==========================================================================");

    Engine::Core::EngineConfig config;
    config.targetFps = 60.0;
    config.fixedDeltaTime = 1.0 / 60.0;
    config.maxFramesToRun = 3;
    config.showStats = false;

    Engine::Core::Engine engine(config);
    engine.initialize();

    LOG_INFO("\n=== INITIAL COMPONENT ARCHETYPE STATES ===");
    engine.getWorld().print_world_state();

    LOG_INFO("\n=== SIMULATING MOVEMENT TICK FOR 3 FRAMES ===");
    engine.run();

    LOG_INFO("\n=== FINAL COMPONENT ARCHETYPE STATES ===");
    engine.getWorld().print_world_state();
}

#include "System/MovementSystem.hpp"
#include "System/PhysicsSystem.hpp"
#include "System/GameplaySystem.hpp"

void runPhase6Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 6 DEMO: DATA-ORIENTED SYSTEM PIPELINE EXECUTION              ");
    LOG_INFO("==========================================================================");

    Engine::Core::EngineConfig config;
    config.targetFps = 60.0;
    config.fixedDeltaTime = 1.0 / 60.0;
    config.maxFramesToRun = 3;
    config.showStats = false;

    Engine::Core::Engine engine(config);
    engine.initialize();

    // Register systems into World pipeline sequence
    LOG_INFO("\n--- REGISTERING SYSTEMS PIPELINE ---");
    engine.getWorld().add_system(std::make_unique<Engine::System::MovementSystem>());
    engine.getWorld().add_system(std::make_unique<Engine::System::PhysicsSystem>());
    engine.getWorld().add_system(std::make_unique<Engine::System::GameplaySystem>());

    using namespace Engine::WorldSystem;
    using namespace Engine::EntitySystem;
    using Vec3 = ::Engine::Math::Vec3;

    // Create a high-speed bullet aimed at the wall
    Entity bullet = engine.getWorld().create_entity("SniperBullet");
    engine.getWorld().add_transform(bullet, Transform(Vec3(90.0, 0.0, 0.0)));
    engine.getWorld().add_velocity(bullet, Velocity(Vec3(300.0, 0.0, 0.0))); // Travels 300 units/s

    LOG_INFO("\n=== INITIAL WORLD ENTITY STATES ===");
    engine.getWorld().print_world_state();

    LOG_INFO("\n=== RUNNING SYSTEM PIPELINE FOR 3 TICKS ===");
    engine.run();

    LOG_INFO("\n=== FINAL WORLD ENTITY STATES ===");
    engine.getWorld().print_world_state();
}

#include "Memory/MemoryBenchmark.hpp"
#include "Resources/AssetManager.hpp"

void runPhase7Demo() {
    Engine::Memory::MemoryBenchmark::runBenchmark(100000);
}

void runPhase8Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 8 DEMO: ASSET MANAGER & RESOURCE CACHING SYSTEM               ");
    LOG_INFO("==========================================================================");

    Engine::Resources::AssetManager assets;

    LOG_INFO("\n--- STEP 1: INITIAL LOAD (CACHE MISS) ---");
    auto level1 = assets.load<Engine::Resources::LevelDataResource>("assets/level01.level");
    if (level1) {
        LOG_INFO("Level Name: " + level1->getLevelName());
        LOG_INFO("Difficulty: " + std::to_string(level1->getDifficulty()));
        std::string entityList;
        for (const auto& e : level1->getInitialEntities()) entityList += e + " ";
        LOG_INFO("Initial Entities: " + entityList);
    }

    LOG_INFO("\n--- STEP 2: RE-REQUEST SAME ASSET (CACHE HIT DEMONSTRATION) ---");
    auto level1_cached = assets.load<Engine::Resources::LevelDataResource>("assets/level01.level");
    if (level1_cached == level1) {
        LOG_INFO("SUCCESS: Verified pointers match! Returned cached asset instantly without disk I/O.");
    }

    LOG_INFO("\n--- STEP 3: LOADING GAME DATA ASSET ---");
    auto playerData = assets.load<Engine::Resources::GameDataResource>("assets/player.data");
    if (playerData) {
        LOG_INFO("Player Hero Name: " + playerData->getValue("name"));
        LOG_INFO("Player Max HP:    " + playerData->getValue("max_hp"));
        LOG_INFO("Player Class:     " + playerData->getValue("class"));
    }

    LOG_INFO("\nTotal Cached Assets in Manager: " + std::to_string(assets.getCachedAssetCount()));
}

#include "Event/EventBus.hpp"

void runPhase9Demo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 9 DEMO: DECOUPLED EVENT BUS & EVENT QUEUE SYSTEM             ");
    LOG_INFO("==========================================================================");

    Engine::EventSystem::EventBus bus;

    LOG_INFO("\n--- STEP 1: REGISTERING EVENT SUBSCRIBERS ---");
    bus.subscribe<Engine::EventSystem::KeyboardEvent>([](const auto& e) {
        LOG_INFO("[Listener -> Input] Keyboard Input Received: Key '" + std::string(1, e.key) + 
                 "' (" + (e.isPressed ? "PRESSED" : "RELEASED") + ")");
    });

    bus.subscribe<Engine::EventSystem::CollisionEvent>([](const auto& e) {
        LOG_INFO("[Listener -> Physics] Collision Detected! Entity [" + std::to_string(e.entityA) + 
                 "] hit Entity [" + std::to_string(e.entityB) + "] at pos " + e.hitPoint.toString());
    });

    bus.subscribe<Engine::EventSystem::EntityCreatedEvent>([](const auto& e) {
        LOG_INFO("[Listener -> World] Entity Spawned: ID " + std::to_string(e.entityId) + 
                 " ('" + e.entityName + "')");
    });

    bus.subscribe<Engine::EventSystem::EntityDestroyedEvent>([](const auto& e) {
        LOG_INFO("[Listener -> World] Entity Destroyed: ID " + std::to_string(e.entityId));
    });

    bus.subscribe<Engine::EventSystem::GameOverEvent>([](const auto& e) {
        LOG_WARN("[Listener -> Gameplay] GAME OVER! Reason: '" + e.reason + 
                 "', Final Score: " + std::to_string(e.finalScore));
    });

    LOG_INFO("\n--- STEP 2: IMMEDIATE EVENT PUBLISHING (publish) ---");
    bus.publish(Engine::EventSystem::EntityCreatedEvent(1, "PlayerHero"));
    bus.publish(Engine::EventSystem::KeyboardEvent('W', true));
    bus.publish(Engine::EventSystem::CollisionEvent(1, 3, Engine::Math::Vec3(100.0, 0.0, 0.0)));

    LOG_INFO("\n--- STEP 3: QUEUED EVENT DEFERRED PROCESSING (enqueue -> dispatchEvents) ---");
    LOG_INFO("Enqueuing events into Event Queue...");
    bus.enqueue(std::make_unique<Engine::EventSystem::KeyboardEvent>(' ', true));
    bus.enqueue(std::make_unique<Engine::EventSystem::EntityDestroyedEvent>(1));
    bus.enqueue(std::make_unique<Engine::EventSystem::GameOverEvent>("Player destroyed in collision", 8500));

    LOG_INFO("Current Queue Size: " + std::to_string(bus.getQueueSize()) + " events pending");
    LOG_INFO("Flushing & Dispatching Event Queue during engine tick...");
    bus.dispatchEvents();
    LOG_INFO("Queue Size after dispatch: " + std::to_string(bus.getQueueSize()) + " events");
}

#include "Debug/Profiler.hpp"

void runProfilerDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 10 DEMO: INSTRUMENTATION PROFILER & TIMING BREAKDOWN        ");
    LOG_INFO("==========================================================================");

    Engine::Core::EngineConfig config;
    config.targetFps = 60.0;
    config.fixedDeltaTime = 1.0 / 60.0;
    config.maxFramesToRun = 10;
    config.showStats = false;

    Engine::Core::Engine engine(config);
    engine.initialize();

    // Register systems into World pipeline sequence
    engine.getWorld().add_system(std::make_unique<Engine::System::MovementSystem>());
    engine.getWorld().add_system(std::make_unique<Engine::System::PhysicsSystem>());
    engine.getWorld().add_system(std::make_unique<Engine::System::GameplaySystem>());

    LOG_INFO("\n=== RUNNING INSTRUMENTED GAME LOOP FOR 10 FRAMES ===");
    engine.run();

    LOG_INFO("\n=== FINAL INSTRUMENTATION PROFILER BREAKDOWN REPORT ===");
    Engine::Debug::Profiler::getInstance().printFrameReport();
}

void runMathDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("                   ENGINE MATH LIBRARY DEMONSTRATION                      ");
    LOG_INFO("==========================================================================");

    using namespace Engine::Math;

    // 1. Vector Operations
    LOG_INFO("\n--- 1. VECTOR OPERATIONS (Vec2, Vec3, Vec4) ---");
    Vec2 v1(3.0, 4.0);
    Vec2 v2(1.0, 2.0);
    LOG_INFO("Vec2 v1: " + v1.toString() + " | Length: " + std::to_string(v1.length()));
    LOG_INFO("Vec2 v1 Normalized: " + v1.normalized().toString());
    LOG_INFO("Vec2 v1 + v2: " + (v1 + v2).toString());
    LOG_INFO("Vec2 Dot Product (v1 . v2): " + std::to_string(dot(v1, v2)));
    LOG_INFO("Vec2 Distance (v1 -> v2): " + std::to_string(distance(v1, v2)));
    LOG_INFO("Vec2 Lerp (v1 -> v2, t=0.5): " + lerp(v1, v2, 0.5).toString());

    Vec3 v3a(1.0, 0.0, 0.0);
    Vec3 v3b(0.0, 1.0, 0.0);
    Vec3 crossProd = cross(v3a, v3b);
    LOG_INFO("Vec3 Cross Product (X x Y): " + crossProd.toString());

    // 2. Matrix Transformations
    LOG_INFO("\n--- 2. MATRIX TRANSFORMATIONS (Mat4) ---");
    Mat4 trans = Mat4::translation(Vec3(10.0, 20.0, 0.0));
    Mat4 rot = Mat4::rotationZ(radians(90.0));
    Mat4 scale = Mat4::scale(Vec3(2.0, 2.0, 1.0));
    Mat4 modelMatrix = trans * rot * scale;

    Vec3 localPoint(1.0, 0.0, 0.0);
    Vec3 worldPoint = modelMatrix.transformPoint(localPoint);
    LOG_INFO("Local Point (1, 0, 0) transformed by Model Matrix: " + worldPoint.toString());

    // 3. 2D Geometry & Bounding Volumes
    LOG_INFO("\n--- 3. 2D GEOMETRY & BOUNDING VOLUMES ---");
    Rectangle rect(0.0, 0.0, 100.0, 50.0);
    Vec2 p1(50.0, 25.0);
    Vec2 p2(150.0, 25.0);
    LOG_INFO("Rectangle [0,0, 100,50] contains Point (50, 25): " + std::string(rect.contains(p1) ? "YES" : "NO"));
    LOG_INFO("Rectangle [0,0, 100,50] contains Point (150, 25): " + std::string(rect.contains(p2) ? "YES" : "NO"));

    Circle circle(Vec2(50.0, 25.0), 30.0);
    LOG_INFO("Circle center (50, 25) r=30 intersects Rectangle [0,0, 100,50]: " + std::string(circle.intersects(rect) ? "YES" : "NO"));

    AABB aabb1 = AABB::fromCenterSize(Vec2(0.0, 0.0), Vec2(10.0, 10.0));
    AABB aabb2 = AABB::fromCenterSize(Vec2(5.0, 5.0), Vec2(10.0, 10.0));
    AABB aabb3 = AABB::fromCenterSize(Vec2(20.0, 20.0), Vec2(10.0, 10.0));
    LOG_INFO("AABB1 [-5..5] intersects AABB2 [0..10]: " + std::string(aabb1.intersects(aabb2) ? "YES" : "NO"));
    LOG_INFO("AABB1 [-5..5] intersects AABB3 [15..25]: " + std::string(aabb1.intersects(aabb3) ? "YES" : "NO"));
    LOG_INFO("==========================================================================");
}

#include "Input/Input.hpp"
#include "Input/TerminalInputBackend.hpp"
#include "State/StateManager.hpp"
#include "State/ConcreteStates.hpp"

void runInputDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("              DECOUPLED INPUT SYSTEM FACADE DEMONSTRATION                 ");
    LOG_INFO("==========================================================================");

    using namespace Engine::InputSystem;

    Input::init();

    LOG_INFO("\n--- 1. TESTING DECOUPLED KEYBOARD API (Input::is_key_*) ---");
    LOG_INFO("Checking Key::W down: " + std::string(Input::is_key_down(Key::W) ? "YES" : "NO"));
    LOG_INFO("Checking Key::Space pressed: " + std::string(Input::is_key_pressed(Key::Space) ? "YES" : "NO"));
    LOG_INFO("Checking Key::Escape released: " + std::string(Input::is_key_released(Key::Escape) ? "YES" : "NO"));

    LOG_INFO("\n--- 2. TESTING DECOUPLED MOUSE API (Input::mouse_*) ---");
    TerminalInputBackend* backend = dynamic_cast<TerminalInputBackend*>(Input::getBackend());
    if (backend) {
        backend->simulateMousePosition(120.5, 450.0);
        backend->simulateMouseButton(MouseButton::Left, true);
    }

    Input::update();

    LOG_INFO("Mouse Position: " + Input::mouse_position().toString());
    LOG_INFO("Mouse Delta:    " + Input::mouse_delta().toString());
    LOG_INFO("Mouse Left Button Down: " + std::string(Input::is_mouse_button_down(MouseButton::Left) ? "YES" : "NO"));
    LOG_INFO("Mouse Right Button Down: " + std::string(Input::is_mouse_button_down(MouseButton::Right) ? "YES" : "NO"));

    Input::shutdown();
    LOG_INFO("==========================================================================");
}

void runStateDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("                   GAME STATE MANAGEMENT DEMONSTRATION                    ");
    LOG_INFO("==========================================================================");

    using namespace Engine::StateSystem;

    StateManager stateManager;

    // Register state instances
    stateManager.registerState(GameState::Menu, std::make_unique<MenuState>(stateManager));
    stateManager.registerState(GameState::Playing, std::make_unique<PlayingState>(stateManager));
    stateManager.registerState(GameState::Paused, std::make_unique<PausedState>(stateManager));
    stateManager.registerState(GameState::GameOver, std::make_unique<GameOverState>(stateManager));

    LOG_INFO("\n--- 1. INITIALIZING GAME TO MAIN MENU ---");
    stateManager.changeState(GameState::Menu);

    LOG_INFO("\n--- 2. TRANSITIONING MENU -> PLAYING ---");
    stateManager.changeState(GameState::Playing);

    LOG_INFO("\n--- 3. SIMULATING PLAYING STATE TICKS (3 seconds to trigger Game Over) ---");
    for (int i = 0; i < 4; ++i) {
        stateManager.update(1.0); // 1-second simulation step per tick
    }

    LOG_INFO("\n--- 4. TRANSITIONING GAME OVER -> PLAYING (Restart) ---");
    stateManager.changeState(GameState::Playing);

    LOG_INFO("\n--- 5. TRANSITIONING PLAYING -> PAUSED ---");
    stateManager.changeState(GameState::Paused);

    LOG_INFO("\n--- 6. TRANSITIONING PAUSED -> MAIN MENU ---");
    stateManager.changeState(GameState::Menu);

    LOG_INFO("==========================================================================");
}

#include "Physics/Collision.hpp"

void runCollisionDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("                 2D COLLISION SYSTEM & EVENT DEMONSTRATION                ");
    LOG_INFO("==========================================================================");

    using namespace Engine::Math;
    using namespace Engine::Physics;
    using namespace Engine::WorldSystem;
    using namespace Engine::EntitySystem;
    using namespace Engine::EventSystem;
    using namespace Engine::System;

    // 1. Core 2D Shape Intersections
    LOG_INFO("\n--- 1. CORE 2D INTERSECTION TESTS ---");
    AABB box1 = AABB::fromCenterSize(Vec2(10.0, 10.0), Vec2(20.0, 20.0));
    AABB box2 = AABB::fromCenterSize(Vec2(15.0, 15.0), Vec2(20.0, 20.0));
    Circle circle1(Vec2(15.0, 15.0), 10.0);
    Vec2 point1(12.0, 12.0);

    LOG_INFO("AABB vs AABB Overlap [Box1 (10,10) & Box2 (15,15)]: " + std::string(Collision::intersects(box1, box2) ? "YES" : "NO"));
    LOG_INFO("Circle vs Circle Overlap [Circle1 r=10 & Circle2 r=10]: " + std::string(Collision::intersects(circle1, Circle(Vec2(20,20), 10)) ? "YES" : "NO"));
    LOG_INFO("Circle vs AABB Intersect [Circle1 (15,15) r=10 & Box1 (10,10)]: " + std::string(Collision::intersects(circle1, box1) ? "YES" : "NO"));
    LOG_INFO("Point vs AABB Intersect [Point (12,12) in Box1 (10,10)]: " + std::string(Collision::intersects(point1, box1) ? "YES" : "NO"));

    // 2. ECS Collision -> CollisionEvent -> GameplaySystem Pipeline
    LOG_INFO("\n--- 2. ECS COLLISION -> EVENT -> GAMEPLAY SYSTEM PIPELINE ---");
    World world;
    EventBus eventBus;

    auto physSystem = std::make_unique<PhysicsSystem>(&eventBus);
    auto gameSystem = std::make_unique<GameplaySystem>(&eventBus);
    gameSystem->setWorld(&world);

    world.add_system(std::move(physSystem));
    world.add_system(std::move(gameSystem));

    // Create Player Entity
    Entity player = world.create_entity("Player");
    world.add_transform(player, Transform(Vec3(10.0, 10.0, 0.0)));
    world.add_collider(player, Collider2D::MakeBox(Vec2(20.0, 20.0)));
    world.add_health(player, Health(100));

    // Create Enemy Entity colliding with Player
    Entity enemy = world.create_entity("Enemy_Boss");
    world.add_transform(enemy, Transform(Vec3(15.0, 15.0, 0.0)));
    world.add_collider(enemy, Collider2D::MakeCircle(10.0));
    world.add_health(enemy, Health(100));

    LOG_INFO("\nExecuting World Update Pipeline (Collision Detection -> Event Dispatch -> Gameplay Reaction):");
    world.update(0.016);
    eventBus.dispatchEvents();

    LOG_INFO("\nPost-Collision Entity Health States:");
    LOG_INFO("Player HP: " + std::to_string(world.get_health(player)->value));
    LOG_INFO("Enemy HP:  " + std::to_string(world.get_health(enemy)->value));
    LOG_INFO("==========================================================================");
}

void runSimplePhysicsDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("                   SIMPLE EULER PHYSICS DEMONSTRATION                     ");
    LOG_INFO("==========================================================================");

    using namespace Engine::WorldSystem;
    using namespace Engine::EntitySystem;
    using namespace Engine::System;
    using namespace Engine::Math;

    World world;
    world.add_system(std::make_unique<MovementSystem>());

    // 1. JUMPING PLAYER
    Entity player = world.create_entity("Jumping_Player");
    world.add_transform(player, Transform(Vec3(0.0, 0.0, 0.0)));
    world.add_velocity(player, Velocity(Vec3(0.0, 15.0, 0.0))); // Initial upward jump impulse
    world.add_rigidbody(player, RigidBody2D(true, 1.0, 0.01, 0.0)); // Gravity active, no bounce

    // 2. BOUNCING BALL
    Entity ball = world.create_entity("Bouncing_Ball");
    world.add_transform(ball, Transform(Vec3(10.0, 20.0, 0.0))); // Dropped from Y=20
    world.add_velocity(ball, Velocity(Vec3(0.0, 0.0, 0.0)));
    world.add_rigidbody(ball, RigidBody2D(true, 1.0, 0.0, 0.8)); // Gravity active, 80% bounciness

    // 3. PROJECTILE
    Entity bullet = world.create_entity("Bullet_Projectile");
    world.add_transform(bullet, Transform(Vec3(0.0, 5.0, 0.0)));
    world.add_velocity(bullet, Velocity(Vec3(30.0, 2.0, 0.0))); // High X velocity, slight Y velocity
    world.add_rigidbody(bullet, RigidBody2D(true, 0.2, 0.0, 0.0)); // Low gravity scale

    // 4. MOVING ENEMY
    Entity enemy = world.create_entity("Patrol_Enemy");
    world.add_transform(enemy, Transform(Vec3(50.0, 0.0, 0.0)));
    world.add_velocity(enemy, Velocity(Vec3(-5.0, 0.0, 0.0)));
    world.add_acceleration(enemy, Acceleration(Vec3(-1.0, 0.0, 0.0))); // Accelerating leftwards

    LOG_INFO("\n--- SIMULATING EULER PHYSICS OVER 5 STEPS (dt = 0.5s) ---");
    for (int step = 1; step <= 5; ++step) {
        world.update(0.5);

        LOG_INFO("\n--- Step " + std::to_string(step) + " (Time = " + std::to_string(step * 0.5) + "s) ---");
        LOG_INFO("Player   - Pos: " + world.get_transform(player)->position.toString() + 
                 " | Vel: " + world.get_velocity(player)->value.toString() + 
                 " | Grounded: " + (world.get_rigidbody(player)->isGrounded ? "YES" : "NO"));

        LOG_INFO("Bouncing Ball - Pos: " + world.get_transform(ball)->position.toString() + 
                 " | Vel: " + world.get_velocity(ball)->value.toString());

        LOG_INFO("Projectile    - Pos: " + world.get_transform(bullet)->position.toString() + 
                 " | Vel: " + world.get_velocity(bullet)->value.toString());

        LOG_INFO("Moving Enemy  - Pos: " + world.get_transform(enemy)->position.toString() + 
                 " | Vel: " + world.get_velocity(enemy)->value.toString());
    }

    LOG_INFO("==========================================================================");
}

#include "Renderer/Renderer.hpp"
#include "System/RenderSystem.hpp"

void runRenderDemo() {
    LOG_INFO("==========================================================================");
    LOG_INFO("               DECOUPLED RENDERING ABSTRACTION DEMONSTRATION              ");
    LOG_INFO("==========================================================================");

    using namespace Engine::RenderSystem;
    using namespace Engine::WorldSystem;
    using namespace Engine::EntitySystem;
    using namespace Engine::System;
    using namespace Engine::Math;

    // 1. Initialize Decoupled Renderer Facade
    Renderer::init(60, 15, "Decoupled Engine Canvas Window");

    LOG_INFO("\n--- 1. DIRECT RENDERING FACADE CALLS ---");
    Renderer::begin_frame();
    Renderer::clear(Color::Black);
    Renderer::draw_rect(AABB::fromCenterSize(Vec2(10.0, 5.0), Vec2(8.0, 4.0)), Color::Green, true);
    Renderer::draw_circle(Circle(Vec2(35.0, 7.0), 4.0), Color::Red, true);
    Renderer::draw_line(Vec2(0.0, 0.0), Vec2(59.0, 14.0), Color::Yellow);
    Renderer::draw_sprite("player_hero", Vec2(48.0, 3.0), Vec2(6.0, 3.0));
    Renderer::end_frame();

    LOG_INFO("\n--- 2. ECS RENDER SYSTEM INTEGRATION ---");
    World world;
    world.add_system(std::make_unique<RenderSystem>());

    // Player Box
    Entity player = world.create_entity("Player_Box");
    world.add_transform(player, Transform(Vec3(8.0, 8.0, 0.0)));
    world.add_collider(player, Collider2D::MakeBox(Vec2(6.0, 4.0)));

    // Enemy Circle
    Entity enemy = world.create_entity("Enemy_Circle");
    world.add_transform(enemy, Transform(Vec3(25.0, 8.0, 0.0)));
    world.add_collider(enemy, Collider2D::MakeCircle(3.0));

    // Sprite Object
    Entity castle = world.create_entity("Castle_Sprite");
    world.add_transform(castle, Transform(Vec3(42.0, 6.0, 0.0)));

    LOG_INFO("Executing ECS RenderSystem Frame Update:");
    world.update(0.016);

    Renderer::shutdown();
    LOG_INFO("==========================================================================");
}

int main(int argc, char* argv[]) {
    Engine::Core::EngineConfig config;
    bool isPhase3Demo = false;
    bool isPhase4Demo = false;
    bool isPhase5Demo = false;
    bool isPhase6Demo = false;
    bool isPhase7Demo = false;
    bool isPhase8Demo = false;
    bool isPhase9Demo = false;
    bool isProfilerDemo = false;
    bool isMathDemo = false;
    bool isInputDemo = false;
    bool isStateDemo = false;
    bool isCollisionDemo = false;
    bool isPhysicsDemo = false;
    bool isRenderDemo = false;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            config.maxFramesToRun = std::stoull(argv[++i]);
        } else if (std::strcmp(argv[i], "--fps") == 0 && i + 1 < argc) {
            config.targetFps = std::stod(argv[++i]);
        } else if (std::strcmp(argv[i], "--fixed-hz") == 0 && i + 1 < argc) {
            double hz = std::stod(argv[++i]);
            if (hz > 0.0) {
                config.fixedDeltaTime = 1.0 / hz;
            }
        } else if (std::strcmp(argv[i], "--log-deltas") == 0) {
            config.logFrameDeltas = true;
        } else if (std::strcmp(argv[i], "--log-fixed") == 0) {
            config.logFixedUpdates = true;
        } else if (std::strcmp(argv[i], "--log-world") == 0) {
            config.logWorldState = true;
        } else if (std::strcmp(argv[i], "--phase2") == 0) {
            config.maxFramesToRun = 5;
            config.logFrameDeltas = true;
            config.showStats = false;
        } else if (std::strcmp(argv[i], "--phase3") == 0) {
            isPhase3Demo = true;
        } else if (std::strcmp(argv[i], "--phase4") == 0) {
            isPhase4Demo = true;
        } else if (std::strcmp(argv[i], "--phase5") == 0) {
            isPhase5Demo = true;
        } else if (std::strcmp(argv[i], "--phase6") == 0) {
            isPhase6Demo = true;
        } else if (std::strcmp(argv[i], "--phase7") == 0) {
            isPhase7Demo = true;
        } else if (std::strcmp(argv[i], "--phase8") == 0) {
            isPhase8Demo = true;
        } else if (std::strcmp(argv[i], "--phase9") == 0) {
            isPhase9Demo = true;
        } else if (std::strcmp(argv[i], "--profiler") == 0) {
            isProfilerDemo = true;
        } else if (std::strcmp(argv[i], "--math") == 0) {
            isMathDemo = true;
        } else if (std::strcmp(argv[i], "--input") == 0) {
            isInputDemo = true;
        } else if (std::strcmp(argv[i], "--state") == 0) {
            isStateDemo = true;
        } else if (std::strcmp(argv[i], "--collision") == 0) {
            isCollisionDemo = true;
        } else if (std::strcmp(argv[i], "--physics") == 0) {
            isPhysicsDemo = true;
        } else if (std::strcmp(argv[i], "--render") == 0) {
            isRenderDemo = true;
        } else if (std::strcmp(argv[i], "--no-stats") == 0) {
            config.showStats = false;
        } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            printHelp(argv[0]);
            return 0;
        }
    }

    if (isPhase3Demo) {
        runPhase3Demo();
        return 0;
    }

    if (isPhase4Demo) {
        runPhase4Demo();
        return 0;
    }

    if (isPhase5Demo) {
        runPhase5Demo();
        return 0;
    }

    if (isPhase6Demo) {
        runPhase6Demo();
        return 0;
    }

    if (isPhase7Demo) {
        runPhase7Demo();
        return 0;
    }

    if (isPhase8Demo) {
        runPhase8Demo();
        return 0;
    }

    if (isPhase9Demo) {
        runPhase9Demo();
        return 0;
    }

    if (isProfilerDemo) {
        runProfilerDemo();
        return 0;
    }

    if (isMathDemo) {
        runMathDemo();
        return 0;
    }

    if (isInputDemo) {
        runInputDemo();
        return 0;
    }

    if (isStateDemo) {
        runStateDemo();
        return 0;
    }

    if (isCollisionDemo) {
        runCollisionDemo();
        return 0;
    }

    if (isPhysicsDemo) {
        runSimplePhysicsDemo();
        return 0;
    }

    if (isRenderDemo) {
        runRenderDemo();
        return 0;
    }

    Engine::Core::Engine engine(config);

    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine core!");
        return 1;
    }

    engine.run();

    return 0;
}
