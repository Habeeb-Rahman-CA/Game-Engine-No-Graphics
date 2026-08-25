#ifndef MATH_VEC4_HPP
#define MATH_VEC4_HPP

#include "Math/MathUtils.hpp"
#include "Math/Vec3.hpp"
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace Math {

struct Vec4 {
    double x;
    double y;
    double z;
    double w;

    constexpr Vec4() : x(0.0), y(0.0), z(0.0), w(1.0) {}
    constexpr Vec4(double xVal, double yVal, double zVal, double wVal = 1.0)
        : x(xVal), y(yVal), z(zVal), w(wVal) {}
    constexpr Vec4(const Vec3& v3, double wVal = 1.0)
        : x(v3.x), y(v3.y), z(v3.z), w(wVal) {}

    // Operators
    Vec4 operator+(const Vec4& other) const { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
    Vec4 operator-(const Vec4& other) const { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
    Vec4 operator*(double scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vec4 operator/(double scalar) const { return Vec4(x / scalar, y / scalar, z / scalar, w / scalar); }

    double lengthSq() const { return x * x + y * y + z * z + w * w; }
    double length() const { return std::sqrt(lengthSq()); }

    Vec4 normalized() const {
        double len = length();
        if (len > 1e-6) {
            return *this / len;
        }
        return Vec4(0.0, 0.0, 0.0, 0.0);
    }

    double dot(const Vec4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
        return ss.str();
    }
};

inline double dot(const Vec4& a, const Vec4& b) {
    return a.dot(b);
}

inline Vec4 lerp(const Vec4& a, const Vec4& b, double t) {
    return Vec4(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t));
}

} // namespace Math
} // namespace Engine

#endif // MATH_VEC4_HPP
