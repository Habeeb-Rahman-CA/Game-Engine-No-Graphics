#include "Memory/MemoryBenchmark.hpp"
#include "Memory/ArenaAllocator.hpp"
#include "Memory/PoolAllocator.hpp"
#include "Entity/Components.hpp"
#include "Debug/Logger.hpp"
#include <chrono>
#include <vector>
#include <cstdlib>
#include <iomanip>

namespace Engine {
namespace Memory {

BenchmarkResults MemoryBenchmark::runBenchmark(size_t numAllocations) {
    BenchmarkResults results;
    using Clock = std::chrono::high_resolution_clock;
    using EntityTransform = EntitySystem::Transform;

    const size_t elementSize = sizeof(EntityTransform);

    LOG_INFO("==========================================================================");
    LOG_INFO("       PHASE 7 BENCHMARK: MALLOC/FREE vs ARENA vs POOL ALLOCATOR          ");
    LOG_INFO("==========================================================================");
    LOG_INFO("Performing " + std::to_string(numAllocations) + " allocations of size " + 
             std::to_string(elementSize) + " bytes...\n");

    // --------------------------------------------------------------------------
    // 1. MALLOC / FREE
    // --------------------------------------------------------------------------
    {
        std::vector<void*> ptrs;
        ptrs.reserve(numAllocations);

        auto start = Clock::now();
        for (size_t i = 0; i < numAllocations; ++i) {
            void* p = std::malloc(elementSize);
            ptrs.push_back(p);
        }
        for (size_t i = 0; i < numAllocations; ++i) {
            std::free(ptrs[i]);
        }
        auto end = Clock::now();
        results.mallocTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    }

    // --------------------------------------------------------------------------
    // 2. ARENA ALLOCATOR
    // --------------------------------------------------------------------------
    {
        size_t totalBytes = (elementSize + alignof(EntityTransform)) * numAllocations;
        ArenaAllocator arena(totalBytes);

        auto start = Clock::now();
        for (size_t i = 0; i < numAllocations; ++i) {
            (void)arena.allocate(elementSize, alignof(EntityTransform));
        }
        arena.reset(); // Instant O(1) deallocation of all items!
        auto end = Clock::now();
        results.arenaTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    }

    // --------------------------------------------------------------------------
    // 3. POOL ALLOCATOR
    // --------------------------------------------------------------------------
    {
        PoolAllocator pool(elementSize, numAllocations);
        std::vector<void*> ptrs;
        ptrs.reserve(numAllocations);

        auto start = Clock::now();
        for (size_t i = 0; i < numAllocations; ++i) {
            void* p = pool.allocate();
            ptrs.push_back(p);
        }
        for (size_t i = 0; i < numAllocations; ++i) {
            pool.free(ptrs[i]);
        }
        auto end = Clock::now();
        results.poolTimeMs = std::chrono::duration<double, std::milli>(end - start).count();
    }

    // Calculate Speedup Ratios
    results.arenaSpeedup = (results.arenaTimeMs > 0) ? (results.mallocTimeMs / results.arenaTimeMs) : 0.0;
    results.poolSpeedup  = (results.poolTimeMs > 0)  ? (results.mallocTimeMs / results.poolTimeMs)  : 0.0;

    LOG_INFO("---------------- BENCHMARK RESULTS SUMMARY ----------------");
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4);
    ss << "1. Standard malloc/free : " << results.mallocTimeMs << " ms (Baseline: 1.00x)\n"
       << "2. ArenaAllocator       : " << results.arenaTimeMs  << " ms (" << std::setprecision(2) << results.arenaSpeedup << "x faster)\n"
       << "3. PoolAllocator        : " << std::setprecision(4) << results.poolTimeMs   << " ms (" << std::setprecision(2) << results.poolSpeedup  << "x faster)";
    LOG_INFO(ss.str());
    LOG_INFO("-----------------------------------------------------------\n");

    return results;
}

} // namespace Memory
} // namespace Engine
