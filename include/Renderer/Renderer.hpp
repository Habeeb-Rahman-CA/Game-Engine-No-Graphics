#ifndef RENDERER_RENDERER_HPP
#define RENDERER_RENDERER_HPP

#include "Renderer/IRendererBackend.hpp"
#include "Renderer/Camera2D.hpp"
#include <memory>

namespace Engine {
namespace RenderSystem {

class Renderer {
public:
    static bool init(int width = 80, int height = 24, const std::string& title = "Game Engine Window");
    static void setBackend(std::unique_ptr<IRendererBackend> backend);
    static IRendererBackend* getBackend();

    static void setCamera(const Camera2D* camera);
    static const Camera2D* getCamera();

    static void begin_frame();
    static void clear(const Color& color = Color::Black);

    static void draw_rect(const Math::AABB& rect, const Color& color = Color::White, bool fill = true);
    static void draw_circle(const Math::Circle& circle, const Color& color = Color::White, bool fill = true);
    static void draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color = Color::White);
    static void draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size);
    static void draw_text(const std::string& text, const Math::Vec2& position, const Color& color = Color::White, float scale = 1.0f);

    static void end_frame();
    static void shutdown();

private:
    static std::unique_ptr<IRendererBackend> s_backend;
    static const Camera2D* s_activeCamera;
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_RENDERER_HPP
