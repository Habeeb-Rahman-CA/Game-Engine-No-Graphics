#ifndef RENDERER_TERMINALCANVASRENDERER_HPP
#define RENDERER_TERMINALCANVASRENDERER_HPP

#include "Renderer/IRendererBackend.hpp"
#include <vector>

namespace Engine {
namespace RenderSystem {

class TerminalCanvasRenderer : public IRendererBackend {
public:
    TerminalCanvasRenderer() = default;
    ~TerminalCanvasRenderer() override = default;

    bool init(int width, int height, const std::string& title) override;
    void begin_frame() override;
    void clear(const Color& color) override;

    void draw_rect(const Math::AABB& rect, const Color& color, bool fill = true) override;
    void draw_circle(const Math::Circle& circle, const Color& color, bool fill = true) override;
    void draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) override;
    void draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) override;

    void end_frame() override;
    void shutdown() override;

private:
    int m_width = 80;
    int m_height = 24;
    std::string m_title;
    std::vector<char> m_buffer;

    void setPixel(int x, int y, char symbol);
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_TERMINALCANVASRENDERER_HPP
