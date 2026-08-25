#include "Renderer/TerminalCanvasRenderer.hpp"
#include "Debug/Logger.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

namespace Engine {
namespace RenderSystem {

bool TerminalCanvasRenderer::init(int width, int height, const std::string& title) {
    m_width = width;
    m_height = height;
    m_title = title;
    m_buffer.assign(m_width * m_height, ' ');
    LOG_INFO("Terminal Canvas Renderer initialized [" + std::to_string(m_width) + "x" + std::to_string(m_height) + "] Window: '" + m_title + "'");
    return true;
}

void TerminalCanvasRenderer::begin_frame() {
    std::fill(m_buffer.begin(), m_buffer.end(), ' ');
}

void TerminalCanvasRenderer::clear(const Color& color) {
    (void)color;
    std::fill(m_buffer.begin(), m_buffer.end(), '.');
}

void TerminalCanvasRenderer::setPixel(int x, int y, char symbol) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_buffer[y * m_width + x] = symbol;
    }
}

void TerminalCanvasRenderer::draw_rect(const Math::AABB& rect, const Color& color, bool fill) {
    (void)color;
    int minX = std::max(0, static_cast<int>(rect.minBound.x));
    int maxX = std::min(m_width - 1, static_cast<int>(rect.maxBound.x));
    int minY = std::max(0, static_cast<int>(rect.minBound.y));
    int maxY = std::min(m_height - 1, static_cast<int>(rect.maxBound.y));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (fill || x == minX || x == maxX || y == minY || y == maxY) {
                setPixel(x, y, '#');
            }
        }
    }
}

void TerminalCanvasRenderer::draw_circle(const Math::Circle& circle, const Color& color, bool fill) {
    (void)color;
    int cx = static_cast<int>(circle.center.x);
    int cy = static_cast<int>(circle.center.y);
    int r  = static_cast<int>(circle.radius);

    for (int y = cy - r; y <= cy + r; ++y) {
        for (int x = cx - r; x <= cx + r; ++x) {
            double distSq = (x - cx) * (x - cx) + (y - cy) * (y - cy);
            if (distSq <= r * r) {
                if (fill || distSq >= (r - 1) * (r - 1)) {
                    setPixel(x, y, 'O');
                }
            }
        }
    }
}

void TerminalCanvasRenderer::draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) {
    (void)color;
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
        setPixel(x0, y0, '*');
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

void TerminalCanvasRenderer::draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) {
    (void)textureId;
    int px = static_cast<int>(position.x);
    int py = static_cast<int>(position.y);
    int w  = static_cast<int>(size.x);
    int h  = static_cast<int>(size.y);

    for (int y = py; y < py + h; ++y) {
        for (int x = px; x < px + w; ++x) {
            setPixel(x, y, '@');
        }
    }
}

void TerminalCanvasRenderer::end_frame() {
    std::cout << "\033[H"; // Move cursor to top-left of terminal
    std::cout << "+" << std::string(m_width, '-') << "+\n";
    for (int y = 0; y < m_height; ++y) {
        std::cout << "|";
        for (int x = 0; x < m_width; ++x) {
            std::cout << m_buffer[y * m_width + x];
        }
        std::cout << "|\n";
    }
    std::cout << "+" << std::string(m_width, '-') << "+\n";
    std::cout.flush();
}

void TerminalCanvasRenderer::shutdown() {
    LOG_INFO("Terminal Canvas Renderer shutdown.");
}

} // namespace RenderSystem
} // namespace Engine
