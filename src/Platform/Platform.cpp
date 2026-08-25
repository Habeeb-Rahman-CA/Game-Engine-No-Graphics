#include "Platform/Platform.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace Platform {

Platform& Platform::getInstance() {
    static Platform instance;
    return instance;
}

bool Platform::initialize(const WindowProps& props) {
    if (m_initialized) return true;

    LOG_INFO("==========================================================================");
    LOG_INFO("               INITIALIZING PLATFORM SUBSYSTEM LAYER                      ");
    LOG_INFO("==========================================================================");

    m_window = std::make_unique<Window>(props);
    if (!m_window->initialize()) {
        LOG_ERROR("Failed to initialize Platform Window!");
        return false;
    }

    m_initialized = true;
    LOG_INFO("Platform Subsystem (Window, Time, Input, FileSystem) Ready!");
    return true;
}

void Platform::update() {
    if (!m_initialized || !m_window) return;

    m_window->poll_events();
    m_window->swap_buffers();
}

void Platform::shutdown() {
    if (!m_initialized) return;

    if (m_window) {
        m_window->close();
        m_window.reset();
    }
    m_initialized = false;
    LOG_INFO("Platform Subsystem Shutdown Complete.");
}

} // namespace Platform
} // namespace Engine
