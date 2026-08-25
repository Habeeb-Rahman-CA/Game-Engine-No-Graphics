#include "Memory/ArenaAllocator.hpp"
#include "Debug/Logger.hpp"
#include <cstdlib>
#include <utility>

namespace Engine {
namespace Memory {

ArenaAllocator::ArenaAllocator(size_t capacityBytes)
    : m_buffer(nullptr), m_capacity(capacityBytes), m_offset(0) {
    if (m_capacity > 0) {
        m_buffer = static_cast<uint8_t*>(std::malloc(m_capacity));
        if (!m_buffer) {
            LOG_ERROR("ArenaAllocator failed to allocate backing store of " + std::to_string(m_capacity) + " bytes");
        }
    }
}

ArenaAllocator::~ArenaAllocator() {
    if (m_buffer) {
        std::free(m_buffer);
        m_buffer = nullptr;
    }
}

ArenaAllocator::ArenaAllocator(ArenaAllocator&& other) noexcept
    : m_buffer(other.m_buffer), m_capacity(other.m_capacity), m_offset(other.m_offset) {
    other.m_buffer = nullptr;
    other.m_capacity = 0;
    other.m_offset = 0;
}

ArenaAllocator& ArenaAllocator::operator=(ArenaAllocator&& other) noexcept {
    if (this != &other) {
        if (m_buffer) std::free(m_buffer);

        m_buffer = other.m_buffer;
        m_capacity = other.m_capacity;
        m_offset = other.m_offset;

        other.m_buffer = nullptr;
        other.m_capacity = 0;
        other.m_offset = 0;
    }
    return *this;
}

void* ArenaAllocator::allocate(size_t bytes, size_t alignment) {
    if (!m_buffer || bytes == 0) return nullptr;

    uintptr_t currentAddr = reinterpret_cast<uintptr_t>(m_buffer + m_offset);
    uintptr_t alignedAddr = (currentAddr + (alignment - 1)) & ~(alignment - 1);
    size_t alignmentOffset = alignedAddr - currentAddr;

    size_t totalNeeded = bytes + alignmentOffset;
    if (m_offset + totalNeeded > m_capacity) {
        LOG_WARN("ArenaAllocator out of memory! Requested: " + std::to_string(bytes) + 
                 " bytes, Available: " + std::to_string(m_capacity - m_offset));
        return nullptr;
    }

    m_offset += totalNeeded;
    return reinterpret_cast<void*>(alignedAddr);
}

void ArenaAllocator::reset() {
    m_offset = 0;
}

} // namespace Memory
} // namespace Engine
