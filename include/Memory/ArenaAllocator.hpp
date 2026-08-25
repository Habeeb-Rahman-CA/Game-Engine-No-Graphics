#ifndef MEMORY_ARENAALLOCATOR_HPP
#define MEMORY_ARENAALLOCATOR_HPP

#include <cstddef>
#include <cstdint>
#include <memory>

namespace Engine {
namespace Memory {

class ArenaAllocator {
public:
    explicit ArenaAllocator(size_t capacityBytes);
    ~ArenaAllocator();

    // Disable copy
    ArenaAllocator(const ArenaAllocator&) = delete;
    ArenaAllocator& operator=(const ArenaAllocator&) = delete;

    // Enable move
    ArenaAllocator(ArenaAllocator&& other) noexcept;
    ArenaAllocator& operator=(ArenaAllocator&& other) noexcept;

    void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t));
    
    template <typename T, typename... Args>
    T* create(Args&&... args) {
        void* ptr = allocate(sizeof(T), alignof(T));
        if (!ptr) return nullptr;
        return new (ptr) T(std::forward<Args>(args)...);
    }

    void reset();

    size_t getUsedBytes() const { return m_offset; }
    size_t getCapacity() const { return m_capacity; }

private:
    uint8_t* m_buffer;
    size_t m_capacity;
    size_t m_offset;
};

} // namespace Memory
} // namespace Engine

#endif // MEMORY_ARENAALLOCATOR_HPP
