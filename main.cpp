#include "Core/Engine.hpp"
#include "Debug/Logger.hpp"
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
              << "  --phase4          Run Phase 4 Entity & Component System demonstration\n"
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

int main(int argc, char* argv[]) {
    Engine::Core::EngineConfig config;
    bool isPhase3Demo = false;
    bool isPhase4Demo = false;
    bool isPhase5Demo = false;
    bool isPhase6Demo = false;
    bool isPhase7Demo = false;
    bool isPhase8Demo = false;
    bool isPhase9Demo = false;

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

    Engine::Core::Engine engine(config);

    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine core!");
        return 1;
    }

    engine.run();

    return 0;
}
