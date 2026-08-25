#ifndef PLATFORM_PLATFORM_HPP
#define PLATFORM_PLATFORM_HPP

#include "Platform/Window.hpp"
#include "Platform/FileSystem.hpp"
#include "Core/Input.hpp"
#include "Core/Time.hpp"
#include <memory>

namespace Engine {
namespace Platform {

class Platform {
public:
    static Platform& getInstance();

    bool initialize(const WindowProps& props = WindowProps());
    void update();
    void shutdown();

    Window* getWindow() { return m_window.get(); }
    Core::Time& getTime() { return m_time; }

private:
    Platform() = default;
    ~Platform() = default;

    std::unique_ptr<Window> m_window;
    Core::Time m_time;
    bool m_initialized = false;
};

} // namespace Platform
} // namespace Engine

#endif // PLATFORM_PLATFORM_HPP
