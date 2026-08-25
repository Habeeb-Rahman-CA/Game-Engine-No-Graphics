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
              << "  --no-stats        Disable periodic render statistics printing\n"
              << "  --help, -h        Show this help message\n";
}

int main(int argc, char* argv[]) {
    Engine::Core::EngineConfig config;

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
        } else if (std::strcmp(argv[i], "--no-stats") == 0) {
            config.showStats = false;
        } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            printHelp(argv[0]);
            return 0;
        }
    }

    Engine::Core::Engine engine(config);

    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize engine core!");
        return 1;
    }

    engine.run();

    return 0;
}
