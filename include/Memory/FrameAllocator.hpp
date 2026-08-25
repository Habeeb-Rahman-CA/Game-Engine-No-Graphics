#ifndef MEMORY_FRAMEALLOCATOR_HPP
#define MEMORY_FRAMEALLOCATOR_HPP

#include "Memory/ArenaAllocator.hpp"

namespace Engine {
namespace Memory {

class FrameAllocator {
public:
    explicit FrameAllocator(size_t capacityBytes) : m_arena(capacityBytes) {}
    ~FrameAllocator() = default;

    void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)) {
        return m_arena.allocate(bytes, alignment);
    }

    template <typename T, typename... Args>
    T* create(Args&&... args) {
        return m_arena.create<T>(std::forward<Args>(args)...);
    }

    void clearFrame() {
        m_arena.reset();
    }

    size_t getUsedBytes() const { return m_arena.getUsedBytes(); }
    size_t getCapacity() const { return m_arena.getCapacity(); }

private:
    ArenaAllocator m_arena;
};

} // namespace Memory
} // namespace Engine

#endif // MEMORY_FRAMEALLOCATOR_HPP
