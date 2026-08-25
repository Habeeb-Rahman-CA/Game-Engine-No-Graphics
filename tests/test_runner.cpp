#include <iostream>

namespace Engine {
namespace Tests {
    bool run_math_tests();
    bool run_allocator_tests();
    bool run_entity_tests();
    bool run_collision_tests();
    bool run_event_tests();
    bool run_serialization_tests();
}
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "               ENGINE UNIT TEST SUITE                   \n";
    std::cout << "========================================================\n";

    bool success = true;

    success &= Engine::Tests::run_math_tests();
    success &= Engine::Tests::run_allocator_tests();
    success &= Engine::Tests::run_entity_tests();
    success &= Engine::Tests::run_collision_tests();
    success &= Engine::Tests::run_event_tests();
    success &= Engine::Tests::run_serialization_tests();

    std::cout << "========================================================\n";
    if (success) {
        std::cout << "ALL ENGINE UNIT TESTS PASSED SUCCESSFULLY! ✓\n";
    } else {
        std::cout << "SOME ENGINE UNIT TESTS FAILED! ✗\n";
    }
    std::cout << "========================================================\n";

    return success ? 0 : 1;
}
