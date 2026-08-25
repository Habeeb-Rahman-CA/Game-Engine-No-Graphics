#include "Memory/PoolAllocator.hpp"
#include "Debug/Logger.hpp"
#include <cstdlib>
#include <algorithm>

namespace Engine {
namespace Memory {

PoolAllocator::PoolAllocator(size_t chunkSize, size_t numChunks)
    : m_buffer(nullptr), m_chunkSize(std::max(chunkSize, sizeof(Node))), 
      m_numChunks(numChunks), m_freeChunks(numChunks), m_freeList(nullptr) {
    
    if (m_numChunks > 0 && m_chunkSize > 0) {
        size_t totalBytes = m_chunkSize * m_numChunks;
        m_buffer = static_cast<uint8_t*>(std::malloc(totalBytes));
        if (!m_buffer) {
            LOG_ERROR("PoolAllocator failed to allocate backing store of " + std::to_string(totalBytes) + " bytes");
            return;
        }
        reset();
    }
}

PoolAllocator::~PoolAllocator() {
    if (m_buffer) {
        std::free(m_buffer);
        m_buffer = nullptr;
    }
}

void PoolAllocator::reset() {
    if (!m_buffer) return;

    m_freeChunks = m_numChunks;
    m_freeList = reinterpret_cast<Node*>(m_buffer);

    Node* current = m_freeList;
    for (size_t i = 0; i < m_numChunks - 1; ++i) {
        uint8_t* nextChunkPtr = reinterpret_cast<uint8_t*>(current) + m_chunkSize;
        current->next = reinterpret_cast<Node*>(nextChunkPtr);
        current = current->next;
    }
    current->next = nullptr;
}

void* PoolAllocator::allocate() {
    if (!m_freeList) {
        LOG_WARN("PoolAllocator out of chunks!");
        return nullptr;
    }

    Node* chunk = m_freeList;
    m_freeList = m_freeList->next;
    m_freeChunks--;

    return reinterpret_cast<void*>(chunk);
}

void PoolAllocator::free(void* ptr) {
    if (!ptr || !m_buffer) return;

    Node* chunk = reinterpret_cast<Node*>(ptr);
    chunk->next = m_freeList;
    m_freeList = chunk;
    m_freeChunks++;
}

} // namespace Memory
} // namespace Engine
