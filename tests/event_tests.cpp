#include "Event/EventBus.hpp"
#include <iostream>
#include <cassert>

namespace Engine {
namespace Tests {

struct TestEvent : public EventSystem::IEvent {
    int value = 0;
    explicit TestEvent(int val) : value(val) {}
    std::string getName() const override { return "TestEvent"; }
};

bool run_event_tests() {
    std::cout << "Running Event System Tests...\n";

    using namespace EventSystem;

    EventBus bus;
    int receivedValue = 0;

    bus.subscribe<TestEvent>([&receivedValue](const TestEvent& e) {
        receivedValue = e.value;
    });

    bus.publish(TestEvent(42));
    assert(receivedValue == 42 && "EventBus publish/subscribe delivery failed!");

    std::cout << "  Event tests      ✓\n";
    return true;
}

} // namespace Tests
} // namespace Engine
