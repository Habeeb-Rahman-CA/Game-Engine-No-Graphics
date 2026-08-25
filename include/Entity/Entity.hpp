#ifndef ENTITY_ENTITY_HPP
#define ENTITY_ENTITY_HPP

#include <cstdint>
#include <limits>

namespace Engine {
namespace EntitySystem {

using Entity = std::uint32_t;
constexpr Entity INVALID_ENTITY = std::numeric_limits<Entity>::max();

} // namespace EntitySystem
} // namespace Engine

#endif // ENTITY_ENTITY_HPP
