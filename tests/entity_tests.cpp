#include "World/World.hpp"
#include <iostream>
#include <cassert>

namespace Engine {
namespace Tests {

bool run_entity_tests() {
    std::cout << "Running Entity & Component Tests...\n";

    using namespace WorldSystem;
    using namespace EntitySystem;

    World world;
    Entity player = world.create_entity("Player_Hero");
    assert(player == 1 && "First entity ID should be 1!");
    assert(world.get_name(player) == "Player_Hero" && "Entity name mismatch!");

    world.add_transform(player, Transform(Math::Vec3(10.0, 20.0, 0.0)));
    world.add_health(player, Health(100, 100));

    assert(world.has_transform(player) && "Entity missing transform component!");
    assert(world.has_health(player) && "Entity missing health component!");

    Transform* t = world.get_transform(player);
    assert(t->position.x == 10.0 && t->position.y == 20.0 && "Transform data mismatch!");

    Health* h = world.get_health(player);
    assert(h->value == 100 && "Health data mismatch!");

    world.destroy_entity(player);
    assert(world.get_entity_count() == 0 && "Entity destruction failed!");

    std::cout << "  Entity tests     ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
