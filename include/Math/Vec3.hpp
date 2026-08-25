#ifndef MATH_VEC3_HPP
#define MATH_VEC3_HPP

#include "Math/MathUtils.hpp"
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace Math {

struct Vec3 {
    double x;
    double y;
    double z;

    constexpr Vec3() : x(0.0), y(0.0), z(0.0) {}
    constexpr Vec3(double xVal, double yVal, double zVal = 0.0) : x(xVal), y(yVal), z(zVal) {}
    explicit constexpr Vec3(double val) : x(val), y(val), z(val) {}

    // Operators
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(double scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
    Vec3 operator/(double scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    Vec3& operator*=(double scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vec3& operator/=(double scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    bool operator==(const Vec3& other) const {
        return std::abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6 && std::abs(z - other.z) < 1e-6;
    }
    bool operator!=(const Vec3& other) const { return !(*this == other); }

    // Vector operations
    double lengthSq() const { return x * x + y * y + z * z; }
    double length() const { return std::sqrt(lengthSq()); }

    Vec3 normalized() const {
        double len = length();
        if (len > 1e-6) {
            return *this / len;
        }
        return Vec3(0.0, 0.0, 0.0);
    }

    void normalize() {
        *this = normalized();
    }

    double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    double distanceSq(const Vec3& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }

    double distance(const Vec3& other) const {
        return std::sqrt(distanceSq(other));
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
};

using Vector3 = Vec3;

// Standalone functions
inline Vec3 operator*(double scalar, const Vec3& vec) {
    return vec * scalar;
}

inline double dot(const Vec3& a, const Vec3& b) {
    return a.dot(b);
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return a.cross(b);
}

inline double distance(const Vec3& a, const Vec3& b) {
    return a.distance(b);
}

inline Vec3 normalize(const Vec3& vec) {
    return vec.normalized();
}

inline Vec3 lerp(const Vec3& a, const Vec3& b, double t) {
    return Vec3(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t));
}

} // namespace Math
} // namespace Engine

#endif // MATH_VEC3_HPP
