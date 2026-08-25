#ifndef RENDERER_GPU2DRENDERER_HPP
#define RENDERER_GPU2DRENDERER_HPP

#include "Renderer/IRendererBackend.hpp"
#include "Platform/Window.hpp"
#include <vector>

namespace Engine {
namespace RenderSystem {

class GPU2DRenderer : public IRendererBackend {
public:
    explicit GPU2DRenderer(Platform::Window* window = nullptr);
    ~GPU2DRenderer() override = default;

    bool init(int width, int height, const std::string& title) override;
    void begin_frame() override;
    void clear(const Color& color) override;

    void draw_rect(const Math::AABB& rect, const Color& color, bool fill = true) override;
    void draw_circle(const Math::Circle& circle, const Color& color, bool fill = true) override;
    void draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) override;
    void draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) override;
    void draw_text(const std::string& text, const Math::Vec2& position, const Color& color, float scale = 1.0f) override;

    void end_frame() override;
    void shutdown() override;

private:
    uint32_t colorToHex(const Color& c);
    Platform::Window* m_window;
    int m_width;
    int m_height;
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_GPU2DRENDERER_HPP
