#ifndef SYSTEM_ISYSTEM_HPP
#define SYSTEM_ISYSTEM_HPP

#include <string>

namespace Engine {

namespace WorldSystem {
class World;
}

namespace System {

class ISystem {
public:
    virtual ~ISystem() = default;
    virtual void update(WorldSystem::World& world, double dt) = 0;
    virtual std::string getName() const = 0;
};

} // namespace System
} // namespace Engine

#endif // SYSTEM_ISYSTEM_HPP
