#ifndef EVENT_EVENTBUS_HPP
#define EVENT_EVENTBUS_HPP

#include "Event/Event.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <memory>
#include <string>

namespace Engine {
namespace EventSystem {

using EventCallback = std::function<void(const IEvent&)>;

class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    // Disable copy
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    // Subscribe to a specific event type T
    template <typename T>
    void subscribe(std::function<void(const T&)> handler) {
        static_assert(std::is_base_of<IEvent, T>::value, "T must derive from IEvent");
        
        std::type_index typeIdx(typeid(T));
        m_subscribers[typeIdx].push_back([handler](const IEvent& event) {
            handler(static_cast<const T&>(event));
        });
        LOG_INFO("[EventBus] Registered subscriber for event type: " + std::string(typeid(T).name()));
    }

    // Publish an event immediately to all subscribers
    template <typename T>
    void publish(const T& event) {
        static_assert(std::is_base_of<IEvent, T>::value, "T must derive from IEvent");

        std::type_index typeIdx(typeid(T));
        auto it = m_subscribers.find(typeIdx);
        if (it != m_subscribers.end()) {
            for (const auto& callback : it->second) {
                callback(event);
            }
        }
    }

    // Enqueue an event for deferred batch processing
    void enqueue(std::unique_ptr<IEvent> event) {
        if (event) {
            m_eventQueue.push_back(std::move(event));
        }
    }

    // Process and dispatch all queued events
    void dispatchEvents() {
        PROFILE_SCOPE("Events");
        if (m_eventQueue.empty()) return;

        std::vector<std::unique_ptr<IEvent>> currentQueue;
        currentQueue.swap(m_eventQueue);

        for (const auto& event : currentQueue) {
            std::type_index typeIdx(typeid(*event));
            auto it = m_subscribers.find(typeIdx);
            if (it != m_subscribers.end()) {
                for (const auto& callback : it->second) {
                    callback(*event);
                }
            }
        }
    }

    size_t getQueueSize() const { return m_eventQueue.size(); }

private:
    std::unordered_map<std::type_index, std::vector<EventCallback>> m_subscribers;
    std::vector<std::unique_ptr<IEvent>> m_eventQueue;
};

} // namespace EventSystem
} // namespace Engine

#endif // EVENT_EVENTBUS_HPP
