#ifndef PLATFORM_WINDOW_HPP
#define PLATFORM_WINDOW_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <memory>

namespace Engine {
namespace Platform {

struct WindowProps {
    std::string title = "Basic Game Engine - Graphical Window";
    uint32_t width = 800;
    uint32_t height = 600;
    bool vsync = true;
    bool fullscreen = false;

    WindowProps(const std::string& t = "Basic Game Engine - Graphical Window", uint32_t w = 800, uint32_t h = 600)
        : title(t), width(w), height(h) {}
};

class Window {
public:
    explicit Window(const WindowProps& props = WindowProps());
    ~Window();

    bool initialize();
    void poll_events();
    void swap_buffers();
    void close();

    bool should_close() const { return m_shouldClose; }
    uint32_t get_width() const { return m_props.width; }
    uint32_t get_height() const { return m_props.height; }
    uint32_t* get_pixel_buffer() { return m_pixelBuffer.data(); }
    void clear_buffer(uint32_t color = 0xFF111118); // Dark Slate BG

    void set_pixel(int x, int y, uint32_t color);

private:
    WindowProps m_props;
    bool m_shouldClose;
    std::vector<uint32_t> m_pixelBuffer;

    // Platform specific native window handles
    void* m_displayHandle; // Display*
    uint64_t m_windowHandle; // Window ID
    void* m_xImage; // XImage*
};

} // namespace Platform
} // namespace Engine

#endif // PLATFORM_WINDOW_HPP
