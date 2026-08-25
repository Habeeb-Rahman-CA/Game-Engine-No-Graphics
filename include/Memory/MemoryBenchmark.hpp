#ifndef MEMORY_MEMORYBENCHMARK_HPP
#define MEMORY_MEMORYBENCHMARK_HPP

#include <cstddef>

namespace Engine {
namespace Memory {

struct BenchmarkResults {
    double mallocTimeMs = 0.0;
    double arenaTimeMs  = 0.0;
    double poolTimeMs   = 0.0;

    double arenaSpeedup = 0.0;
    double poolSpeedup  = 0.0;
};

class MemoryBenchmark {
public:
    static BenchmarkResults runBenchmark(size_t numAllocations = 100000);
};

} // namespace Memory
} // namespace Engine

#endif // MEMORY_MEMORYBENCHMARK_HPP
