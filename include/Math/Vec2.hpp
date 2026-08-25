#ifndef MATH_VEC2_HPP
#define MATH_VEC2_HPP

#include "Math/MathUtils.hpp"
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace Math {

struct Vec2 {
    double x;
    double y;

    constexpr Vec2() : x(0.0), y(0.0) {}
    constexpr Vec2(double xVal, double yVal) : x(xVal), y(yVal) {}
    explicit constexpr Vec2(double val) : x(val), y(val) {}

    // Operators
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(double scalar) const { return Vec2(x * scalar, y * scalar); }
    Vec2 operator/(double scalar) const { return Vec2(x / scalar, y / scalar); }
    Vec2 operator-() const { return Vec2(-x, -y); }

    Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
    Vec2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator/=(double scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const Vec2& other) const { return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }

    // Vector operations
    double lengthSq() const { return x * x + y * y; }
    double length() const { return std::sqrt(lengthSq()); }

    Vec2 normalized() const {
        double len = length();
        if (len > 1e-6) {
            return *this / len;
        }
        return Vec2(0.0, 0.0);
    }

    void normalize() {
        *this = normalized();
    }

    double dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    double distanceSq(const Vec2& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return dx * dx + dy * dy;
    }

    double distance(const Vec2& other) const {
        return std::sqrt(distanceSq(other));
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }
};

// Standalone functions
inline Vec2 operator*(double scalar, const Vec2& vec) {
    return vec * scalar;
}

inline double dot(const Vec2& a, const Vec2& b) {
    return a.dot(b);
}

inline double distance(const Vec2& a, const Vec2& b) {
    return a.distance(b);
}

inline Vec2 normalize(const Vec2& vec) {
    return vec.normalized();
}

inline Vec2 lerp(const Vec2& a, const Vec2& b, double t) {
    return Vec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

} // namespace Math
} // namespace Engine

#endif // MATH_VEC2_HPP
