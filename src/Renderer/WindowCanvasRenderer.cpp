#include "Renderer/WindowCanvasRenderer.hpp"
#include "Debug/Logger.hpp"
#include <cmath>
#include <algorithm>

namespace Engine {
namespace RenderSystem {

WindowCanvasRenderer::WindowCanvasRenderer(Platform::Window* window)
    : m_window(window), m_width(800), m_height(600) {
    if (window) {
        m_width = window->get_width();
        m_height = window->get_height();
    }
}

uint32_t WindowCanvasRenderer::colorToHex(const Color& c) {
    uint32_t r = static_cast<uint32_t>(c.r * 255.0f);
    uint32_t g = static_cast<uint32_t>(c.g * 255.0f);
    uint32_t b = static_cast<uint32_t>(c.b * 255.0f);
    return (r << 16) | (g << 8) | b;
}

bool WindowCanvasRenderer::init(int width, int height, const std::string& title) {
    m_width = width;
    m_height = height;
    LOG_INFO("WindowCanvasRenderer Backend Initialized (" + title + ")");
    return true;
}

void WindowCanvasRenderer::begin_frame() {
    if (m_window) {
        m_window->clear_buffer(0xFF111118); // Dark BG
    }
}

void WindowCanvasRenderer::clear(const Color& color) {
    if (m_window) {
        m_window->clear_buffer(colorToHex(color));
    }
}

void WindowCanvasRenderer::draw_rect(const Math::AABB& rect, const Color& color, bool fill) {
    if (!m_window) return;

    uint32_t hexColor = colorToHex(color);
    int minX = static_cast<int>(rect.minBound.x);
    int minY = static_cast<int>(rect.minBound.y);
    int maxX = static_cast<int>(rect.maxBound.x);
    int maxY = static_cast<int>(rect.maxBound.y);

    if (fill) {
        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                m_window->set_pixel(x, y, hexColor);
            }
        }
    } else {
        for (int x = minX; x <= maxX; ++x) {
            m_window->set_pixel(x, minY, hexColor);
            m_window->set_pixel(x, maxY, hexColor);
        }
        for (int y = minY; y <= maxY; ++y) {
            m_window->set_pixel(minX, y, hexColor);
            m_window->set_pixel(maxX, y, hexColor);
        }
    }
}

void WindowCanvasRenderer::draw_circle(const Math::Circle& circle, const Color& color, bool fill) {
    if (!m_window) return;

    uint32_t hexColor = colorToHex(color);
    int cx = static_cast<int>(circle.center.x);
    int cy = static_cast<int>(circle.center.y);
    int r = static_cast<int>(circle.radius);

    for (int y = -r; y <= r; ++y) {
        for (int x = -r; x <= r; ++x) {
            if (fill) {
                if (x * x + y * y <= r * r) {
                    m_window->set_pixel(cx + x, cy + y, hexColor);
                }
            } else {
                int distSq = x * x + y * y;
                if (distSq >= (r - 1) * (r - 1) && distSq <= r * r) {
                    m_window->set_pixel(cx + x, cy + y, hexColor);
                }
            }
        }
    }
}

void WindowCanvasRenderer::draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) {
    if (!m_window) return;

    uint32_t hexColor = colorToHex(color);
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        m_window->set_pixel(x0, y0, hexColor);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

void WindowCanvasRenderer::draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) {
    (void)textureId;
    Math::AABB box = Math::AABB::fromCenterSize(position, size);
    draw_rect(box, Color::Green, true);
}

void WindowCanvasRenderer::end_frame() {
    if (m_window) {
        m_window->swap_buffers();
    }
}

void WindowCanvasRenderer::shutdown() {
    LOG_INFO("WindowCanvasRenderer Shutdown.");
}

} // namespace RenderSystem
} // namespace Engine
