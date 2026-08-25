#include "World/World.hpp"
#include "System/MovementSystem.hpp"
#include "Physics/Collision.hpp"
#include "Memory/ArenaAllocator.hpp"
#include "Memory/PoolAllocator.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

namespace Engine {
namespace Benchmark {

struct BenchmarkResult {
    std::string testName;
    size_t count;
    double durationMs;
    double opsPerSec;
};

class Timer {
public:
    void start() { m_start = std::chrono::high_resolution_clock::now(); }
    double stopMs() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - m_start;
        return elapsed.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_start;
};

void run_entity_creation_benchmark(std::vector<BenchmarkResult>& results) {
    std::vector<size_t> counts = { 1000, 10000, 100000 };

    for (size_t count : counts) {
        WorldSystem::World world;
        Timer timer;
        timer.start();

        for (size_t i = 0; i < count; ++i) {
            EntitySystem::Entity e = world.create_entity("Entity_" + std::to_string(i));
            world.add_transform(e, EntitySystem::Transform(Math::Vec3(i * 1.0, i * 2.0, 0.0)));
            world.add_velocity(e, EntitySystem::Velocity(Math::Vec3(1.0, 1.0, 0.0)));
            world.add_health(e, EntitySystem::Health(100, 100));
        }

        double ms = timer.stopMs();
        double ops = (count / (ms / 1000.0));
        results.push_back({ "Entity Creation (" + std::to_string(count) + ")", count, ms, ops });
    }
}

void run_system_update_benchmark(std::vector<BenchmarkResult>& results) {
    std::vector<size_t> counts = { 1000, 10000, 100000 };

    for (size_t count : counts) {
        WorldSystem::World world;
        world.add_system(std::make_unique<System::MovementSystem>());

        for (size_t i = 0; i < count; ++i) {
            EntitySystem::Entity e = world.create_entity();
            world.add_transform(e, EntitySystem::Transform(Math::Vec3(i * 1.0, i * 2.0, 0.0)));
            world.add_velocity(e, EntitySystem::Velocity(Math::Vec3(1.5, 0.5, 0.0)));
        }

        Timer timer;
        timer.start();
        world.update(0.016);
        double ms = timer.stopMs();

        double ops = (count / (ms / 1000.0));
        results.push_back({ "System Update (" + std::to_string(count) + ")", count, ms, ops });
    }
}

void run_collision_benchmark(std::vector<BenchmarkResult>& results) {
    std::vector<size_t> counts = { 1000, 5000, 10000 };

    for (size_t count : counts) {
        std::vector<Math::AABB> boxes;
        boxes.reserve(count);

        for (size_t i = 0; i < count; ++i) {
            boxes.push_back(Math::AABB(Math::Vec2(i * 0.5, i * 0.5), Math::Vec2(i * 0.5 + 2.0, i * 0.5 + 2.0)));
        }

        Timer timer;
        timer.start();
        size_t collisionCount = 0;

        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (boxes[i].intersects(boxes[j])) {
                    collisionCount++;
                }
            }
        }

        double ms = timer.stopMs();
        double ops = (count / (ms / 1000.0));
        results.push_back({ "Collision Checks (" + std::to_string(count) + " boxes, " + std::to_string(collisionCount) + " hits)", count, ms, ops });
    }
}

void run_allocator_benchmark(std::vector<BenchmarkResult>& results) {
    size_t count = 100000;

    // Standard malloc/free
    {
        Timer timer;
        timer.start();
        std::vector<void*> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            ptrs.push_back(malloc(64));
        }
        for (void* p : ptrs) free(p);
        double ms = timer.stopMs();
        results.push_back({ "Standard Malloc/Free (100k)", count, ms, (count / (ms / 1000.0)) });
    }

    // Arena Allocator
    {
        Timer timer;
        timer.start();
        Memory::ArenaAllocator arena(64 * count + 1024);
        for (size_t i = 0; i < count; ++i) {
            arena.allocate(64);
        }
        arena.reset();
        double ms = timer.stopMs();
        results.push_back({ "Arena Allocator (100k)", count, ms, (count / (ms / 1000.0)) });
    }

    // Pool Allocator
    {
        Timer timer;
        timer.start();
        Memory::PoolAllocator pool(64, count);
        std::vector<void*> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            ptrs.push_back(pool.allocate());
        }
        for (void* p : ptrs) pool.free(p);
        double ms = timer.stopMs();
        results.push_back({ "Pool Allocator (100k)", count, ms, (count / (ms / 1000.0)) });
    }
}

void print_report(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n=========================================================================================\n";
    std::cout << "                                ENGINE BENCHMARK REPORT                                  \n";
    std::cout << "=========================================================================================\n";
    std::cout << std::left << std::setw(45) << "BENCHMARK TEST" 
              << std::right << std::setw(15) << "COUNT" 
              << std::right << std::setw(15) << "TIME (ms)" 
              << std::right << std::setw(18) << "THROUGHPUT (ops/s)" << "\n";
    std::cout << "-----------------------------------------------------------------------------------------\n";

    for (const auto& r : results) {
        std::cout << std::left << std::setw(45) << r.testName
                  << std::right << std::setw(15) << r.count
                  << std::right << std::setw(15) << std::fixed << std::setprecision(3) << r.durationMs
                  << std::right << std::setw(18) << std::fixed << std::setprecision(0) << r.opsPerSec << "\n";
    }
    std::cout << "=========================================================================================\n";
}

} // namespace Benchmark
} // namespace Engine

#include "Debug/Logger.hpp"

int main() {
    Engine::Debug::Logger::setLogLevel(Engine::Debug::LogLevel::Warning);

    std::vector<Engine::Benchmark::BenchmarkResult> results;

    std::cout << "Starting Engine Subsystem Performance Benchmarks...\n";

    Engine::Benchmark::run_entity_creation_benchmark(results);
    Engine::Benchmark::run_system_update_benchmark(results);
    Engine::Benchmark::run_collision_benchmark(results);
    Engine::Benchmark::run_allocator_benchmark(results);

    Engine::Benchmark::print_report(results);

    return 0;
}
