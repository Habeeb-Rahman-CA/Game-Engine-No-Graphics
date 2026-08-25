#ifndef RENDERER_IRENDERERBACKEND_HPP
#define RENDERER_IRENDERERBACKEND_HPP

#include "Renderer/Color.hpp"
#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include <string>

namespace Engine {
namespace RenderSystem {

class IRendererBackend {
public:
    virtual ~IRendererBackend() = default;

    virtual bool init(int width, int height, const std::string& title) = 0;
    virtual void begin_frame() = 0;
    virtual void clear(const Color& color) = 0;

    virtual void draw_rect(const Math::AABB& rect, const Color& color, bool fill = true) = 0;
    virtual void draw_circle(const Math::Circle& circle, const Color& color, bool fill = true) = 0;
    virtual void draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) = 0;
    virtual void draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) = 0;
    virtual void draw_text(const std::string& text, const Math::Vec2& position, const Color& color, float scale = 1.0f) = 0;

    virtual void end_frame() = 0;
    virtual void shutdown() = 0;
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_IRENDERERBACKEND_HPP
