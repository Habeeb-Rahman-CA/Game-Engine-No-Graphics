#include "Renderer/Renderer.hpp"
#include "Renderer/TerminalCanvasRenderer.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace RenderSystem {

std::unique_ptr<IRendererBackend> Renderer::s_backend = nullptr;
const Camera2D* Renderer::s_activeCamera = nullptr;

bool Renderer::init(int width, int height, const std::string& title) {
    if (!s_backend) {
        s_backend = std::make_unique<TerminalCanvasRenderer>();
    }
    LOG_INFO("Engine Renderer Facade initialized with backend: " + title);
    return s_backend->init(width, height, title);
}

void Renderer::setBackend(std::unique_ptr<IRendererBackend> backend) {
    if (s_backend) {
        s_backend->shutdown();
    }
    s_backend = std::move(backend);
}

IRendererBackend* Renderer::getBackend() {
    return s_backend.get();
}

void Renderer::setCamera(const Camera2D* camera) {
    s_activeCamera = camera;
}

const Camera2D* Renderer::getCamera() {
    return s_activeCamera;
}

void Renderer::begin_frame() {
    if (s_backend) s_backend->begin_frame();
}

void Renderer::clear(const Color& color) {
    if (s_backend) s_backend->clear(color);
}

void Renderer::draw_rect(const Math::AABB& rect, const Color& color, bool fill) {
    if (!s_backend) return;
    if (s_activeCamera) {
        Math::AABB screenRect = s_activeCamera->worldToScreen(rect);
        s_backend->draw_rect(screenRect, color, fill);
    } else {
        s_backend->draw_rect(rect, color, fill);
    }
}

void Renderer::draw_circle(const Math::Circle& circle, const Color& color, bool fill) {
    if (!s_backend) return;
    if (s_activeCamera) {
        Math::Circle screenCircle = s_activeCamera->worldToScreen(circle);
        s_backend->draw_circle(screenCircle, color, fill);
    } else {
        s_backend->draw_circle(circle, color, fill);
    }
}

void Renderer::draw_line(const Math::Vec2& start, const Math::Vec2& end, const Color& color) {
    if (!s_backend) return;
    if (s_activeCamera) {
        Math::Vec2 screenStart = s_activeCamera->worldToScreen(start);
        Math::Vec2 screenEnd   = s_activeCamera->worldToScreen(end);
        s_backend->draw_line(screenStart, screenEnd, color);
    } else {
        s_backend->draw_line(start, end, color);
    }
}

void Renderer::draw_sprite(const std::string& textureId, const Math::Vec2& position, const Math::Vec2& size) {
    if (!s_backend) return;
    if (s_activeCamera) {
        Math::Vec2 screenPos  = s_activeCamera->worldToScreen(position);
        Math::Vec2 screenSize = size * s_activeCamera->zoom;
        s_backend->draw_sprite(textureId, screenPos, screenSize);
    } else {
        s_backend->draw_sprite(textureId, position, size);
    }
}

void Renderer::end_frame() {
    if (s_backend) s_backend->end_frame();
}

void Renderer::shutdown() {
    if (s_backend) {
        s_backend->shutdown();
        s_backend.reset();
    }
    s_activeCamera = nullptr;
    LOG_INFO("Engine Renderer Facade shutdown.");
}

} // namespace RenderSystem
} // namespace Engine
