#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/Geometry.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

namespace Engine {
namespace Tests {

bool run_math_tests() {
    std::cout << "Running Math Tests...\n";

    // 1. Vec2 Operations
    Math::Vec2 v1(3.0, 4.0);
    assert(std::abs(v1.length() - 5.0) < 0.0001 && "Vec2 length calculation failed!");
    
    Math::Vec2 v2(1.0, 2.0);
    Math::Vec2 sum = v1 + v2;
    assert(sum.x == 4.0 && sum.y == 6.0 && "Vec2 addition failed!");

    double dot = v1.dot(v2);
    assert(dot == 11.0 && "Vec2 dot product failed!");

    Math::Vec2 norm = v1.normalized();
    assert(std::abs(norm.length() - 1.0) < 0.0001 && "Vec2 normalization failed!");

    // 2. Vec3 Operations
    Math::Vec3 v3a(1.0, 2.0, 3.0);
    Math::Vec3 v3b(4.0, 5.0, 6.0);
    Math::Vec3 v3sum = v3a + v3b;
    assert(v3sum.x == 5.0 && v3sum.y == 7.0 && v3sum.z == 9.0 && "Vec3 addition failed!");

    // 3. AABB Intersections
    Math::AABB box1(Math::Vec2(0.0, 0.0), Math::Vec2(10.0, 10.0));
    Math::AABB box2(Math::Vec2(5.0, 5.0), Math::Vec2(15.0, 15.0));
    Math::AABB box3(Math::Vec2(20.0, 20.0), Math::Vec2(30.0, 30.0));

    assert(box1.intersects(box2) && "AABB box1 vs box2 intersection failed!");
    assert(!box1.intersects(box3) && "AABB box1 vs box3 non-intersection failed!");

    std::cout << "  Vec2 tests       ✓\n";
    std::cout << "  Vec3 tests       ✓\n";
    std::cout << "  AABB tests       ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
