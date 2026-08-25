#include "Physics/Collision.hpp"
#include <iostream>
#include <cassert>

namespace Engine {
namespace Tests {

bool run_collision_tests() {
    std::cout << "Running Collision Tests...\n";

    using namespace Physics;
    using namespace Math;

    // 1. AABB vs AABB
    AABB box1(Vec2(0.0, 0.0), Vec2(10.0, 10.0));
    AABB box2(Vec2(5.0, 5.0), Vec2(15.0, 15.0));
    AABB box3(Vec2(20.0, 20.0), Vec2(30.0, 30.0));

    assert(Collision::intersects(box1, box2) && "AABB vs AABB collision check failed!");
    assert(!Collision::intersects(box1, box3) && "AABB vs AABB separation check failed!");

    // 2. Circle vs Circle
    Circle circle1(Vec2(0.0, 0.0), 5.0);
    Circle circle2(Vec2(6.0, 0.0), 5.0);
    Circle circle3(Vec2(20.0, 0.0), 5.0);

    assert(Collision::intersects(circle1, circle2) && "Circle vs Circle collision check failed!");
    assert(!Collision::intersects(circle1, circle3) && "Circle vs Circle separation check failed!");

    // 3. Circle vs AABB
    assert(Collision::intersects(circle1, box1) && "Circle vs AABB collision check failed!");

    std::cout << "  Collision tests  ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
