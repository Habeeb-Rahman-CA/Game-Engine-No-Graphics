#ifndef MATH_VEC3_HPP
#define MATH_VEC3_HPP

#include <iostream>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace Math {

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    Vec3(double xVal = 0.0, double yVal = 0.0, double zVal = 0.0)
        : x(xVal), y(yVal), z(zVal) {}

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator*(double scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
};

using Vector3 = Vec3;

} // namespace Math
} // namespace Engine

#endif // MATH_VEC3_HPP
