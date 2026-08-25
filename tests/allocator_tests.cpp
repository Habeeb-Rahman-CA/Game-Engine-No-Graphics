#include "Memory/ArenaAllocator.hpp"
#include "Memory/PoolAllocator.hpp"
#include <iostream>
#include <cassert>

namespace Engine {
namespace Tests {

bool run_allocator_tests() {
    std::cout << "Running Allocator Tests...\n";

    using namespace Memory;

    // 1. Arena Allocator
    ArenaAllocator arena(1024);
    void* ptr1 = arena.allocate(128, 8);
    assert(ptr1 != nullptr && "Arena allocation failed!");
    assert(arena.getUsedBytes() >= 128 && "Arena used memory tracking failed!");

    void* ptr2 = arena.allocate(256, 8);
    assert(ptr2 != nullptr && "Arena second allocation failed!");

    arena.reset();
    assert(arena.getUsedBytes() == 0 && "Arena reset failed!");

    // 2. Pool Allocator
    PoolAllocator pool(64, 10);
    void* block1 = pool.allocate();
    assert(block1 != nullptr && "Pool allocation failed!");

    void* block2 = pool.allocate();
    assert(block2 != nullptr && "Pool second allocation failed!");

    pool.free(block1);
    pool.free(block2);

    std::cout << "  Arena tests      ✓\n";
    std::cout << "  Pool tests       ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
