#ifndef RENDERER_CAMERA2D_HPP
#define RENDERER_CAMERA2D_HPP

#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"

namespace Engine {
namespace RenderSystem {

using Vec2 = Math::Vec2;
using AABB = Math::AABB;
using Circle = Math::Circle;

class Camera2D {
public:
    Vec2 position;
    double zoom = 1.0;
    Vec2 viewportSize;

    Camera2D(Vec2 pos = Vec2(0.0, 0.0), double zoomFactor = 1.0, Vec2 viewport = Vec2(80.0, 24.0))
        : position(pos), zoom(zoomFactor), viewportSize(viewport) {}

    // Coordinate Transformations
    Vec2 worldToScreen(const Vec2& worldPos) const {
        Vec2 centerOffset = viewportSize * 0.5;
        return (worldPos - position) * zoom + centerOffset;
    }

    Vec2 screenToWorld(const Vec2& screenPos) const {
        Vec2 centerOffset = viewportSize * 0.5;
        return (screenPos - centerOffset) * (1.0 / zoom) + position;
    }

    AABB worldToScreen(const AABB& worldBox) const {
        Vec2 minScreen = worldToScreen(worldBox.minBound);
        Vec2 maxScreen = worldToScreen(worldBox.maxBound);
        return AABB(minScreen, maxScreen);
    }

    Circle worldToScreen(const Circle& worldCircle) const {
        Vec2 centerScreen = worldToScreen(worldCircle.center);
        double radiusScreen = worldCircle.radius * zoom;
        return Circle(centerScreen, radiusScreen);
    }
};

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_CAMERA2D_HPP
