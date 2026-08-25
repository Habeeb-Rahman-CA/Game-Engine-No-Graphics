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
              << "  --phase2          Run Phase 2 Time System demonstration\n"
              << "  --phase3          Run Phase 3 Fixed Timestep demonstration (Comparing Render FPS vs Fixed Simulation Hz)\n"
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

int main(int argc, char* argv[]) {
    Engine::Core::EngineConfig config;
    bool isPhase3Demo = false;

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
        } else if (std::strcmp(argv[i], "--phase2") == 0) {
            config.maxFramesToRun = 5;
            config.logFrameDeltas = true;
            config.showStats = false;
        } else if (std::strcmp(argv[i], "--phase3") == 0) {
            isPhase3Demo = true;
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

    Engine::Core::Engine engine(config);

    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine core!");
        return 1;
    }

    engine.run();

    return 0;
}
