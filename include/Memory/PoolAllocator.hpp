#ifndef MEMORY_POOLALLOCATOR_HPP
#define MEMORY_POOLALLOCATOR_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

namespace Engine {
namespace Memory {

class PoolAllocator {
public:
    PoolAllocator(size_t chunkSize, size_t numChunks);
    ~PoolAllocator();

    // Disable copy
    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;

    void* allocate();
    void free(void* ptr);
    void reset();

    template <typename T, typename... Args>
    T* create(Args&&... args) {
        static_assert(sizeof(T) <= 256, "Object size exceeds max pool chunk size assumption");
        void* ptr = allocate();
        if (!ptr) return nullptr;
        return new (ptr) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void destroy(T* ptr) {
        if (!ptr) return;
        ptr->~T();
        free(ptr);
    }

    size_t getChunkSize() const { return m_chunkSize; }
    size_t getCapacity() const { return m_numChunks; }
    size_t getFreeChunkCount() const { return m_freeChunks; }

private:
    struct Node {
        Node* next;
    };

    uint8_t* m_buffer;
    size_t m_chunkSize;
    size_t m_numChunks;
    size_t m_freeChunks;
    Node* m_freeList;
};

} // namespace Memory
} // namespace Engine

#endif // MEMORY_POOLALLOCATOR_HPP
