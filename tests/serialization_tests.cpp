#include "Scene/SceneSerializer.hpp"
#include "World/World.hpp"
#include <iostream>
#include <cassert>

namespace Engine {
namespace Tests {

bool run_serialization_tests() {
    std::cout << "Running Serialization Tests...\n";

    using namespace WorldSystem;
    using namespace EntitySystem;

    World world;
    Entity e1 = world.create_entity("Test_Hero");
    world.add_transform(e1, Transform(Math::Vec3(15.0, 30.0, 0.0)));
    world.add_health(e1, Health(75, 100));

    assert(world.save_scene("test_saved.json") && "Saving scene to file failed!");

    World loadedWorld;
    assert(loadedWorld.load_scene("test_saved.json") && "Loading saved scene file failed!");
    assert(loadedWorld.get_entity_count() == 1 && "Loaded scene entity count mismatch!");

    Entity loadedE = loadedWorld.get_entities()[0];
    assert(loadedWorld.get_name(loadedE) == "Test_Hero" && "Loaded entity name mismatch!");
    assert(loadedWorld.get_health(loadedE)->value == 75 && "Loaded entity health mismatch!");

    std::cout << "  Serialization tests ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
