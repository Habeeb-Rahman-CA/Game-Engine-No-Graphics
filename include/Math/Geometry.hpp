#ifndef MATH_GEOMETRY_HPP
#define MATH_GEOMETRY_HPP

#include "Math/Vec2.hpp"
#include "Math/Vec3.hpp"
#include "Math/MathUtils.hpp"
#include <algorithm>

namespace Engine {
namespace Math {

struct Rectangle {
    double x;
    double y;
    double width;
    double height;

    constexpr Rectangle() : x(0.0), y(0.0), width(0.0), height(0.0) {}
    constexpr Rectangle(double xVal, double yVal, double wVal, double hVal)
        : x(xVal), y(yVal), width(wVal), height(hVal) {}
    constexpr Rectangle(const Vec2& pos, const Vec2& size)
        : x(pos.x), y(pos.y), width(size.x), height(size.y) {}

    bool contains(const Vec2& point) const {
        return point.x >= x && point.x <= (x + width) &&
               point.y >= y && point.y <= (y + height);
    }

    bool intersects(const Rectangle& other) const {
        return x < (other.x + other.width) && (x + width) > other.x &&
               y < (other.y + other.height) && (y + height) > other.y;
    }
};

struct Circle {
    Vec2 center;
    double radius;

    constexpr Circle() : center(0.0, 0.0), radius(0.0) {}
    constexpr Circle(const Vec2& c, double r) : center(c), radius(r) {}
    constexpr Circle(double cx, double cy, double r) : center(cx, cy), radius(r) {}

    bool contains(const Vec2& point) const {
        return center.distanceSq(point) <= (radius * radius);
    }

    bool intersects(const Circle& other) const {
        double combinedRadius = radius + other.radius;
        return center.distanceSq(other.center) <= (combinedRadius * combinedRadius);
    }

    bool intersects(const Rectangle& rect) const {
        // Find nearest point on rectangle to circle center
        double nearestX = clamp(center.x, rect.x, rect.x + rect.width);
        double nearestY = clamp(center.y, rect.y, rect.y + rect.height);
        Vec2 nearestPoint(nearestX, nearestY);

        return contains(nearestPoint);
    }
};

struct AABB {
    Vec2 minBound;
    Vec2 maxBound;

    constexpr AABB() : minBound(0.0, 0.0), maxBound(0.0, 0.0) {}
    constexpr AABB(const Vec2& minB, const Vec2& maxB) : minBound(minB), maxBound(maxB) {}

    static AABB fromCenterSize(const Vec2& center, const Vec2& size) {
        Vec2 halfSize = size * 0.5;
        return AABB(center - halfSize, center + halfSize);
    }

    bool contains(const Vec2& point) const {
        return point.x >= minBound.x && point.x <= maxBound.x &&
               point.y >= minBound.y && point.y <= maxBound.y;
    }

    bool intersects(const AABB& other) const {
        return minBound.x <= other.maxBound.x && maxBound.x >= other.minBound.x &&
               minBound.y <= other.maxBound.y && maxBound.y >= other.minBound.y;
    }

    void expand(const Vec2& point) {
        minBound.x = std::min(minBound.x, point.x);
        minBound.y = std::min(minBound.y, point.y);
        maxBound.x = std::max(maxBound.x, point.x);
        maxBound.y = std::max(maxBound.y, point.y);
    }
};

} // namespace Math
} // namespace Engine

#endif // MATH_GEOMETRY_HPP
