#ifndef MATH_MAT4_HPP
#define MATH_MAT4_HPP

#include "Math/Vec3.hpp"
#include "Math/Vec4.hpp"
#include "Math/MathUtils.hpp"
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

namespace Engine {
namespace Math {

struct Mat4 {
    // Column-major 4x4 matrix: m[col][row]
    double m[4][4];

    Mat4() {
        setIdentity();
    }

    static Mat4 identity() {
        Mat4 result;
        result.setIdentity();
        return result;
    }

    void setIdentity() {
        std::memset(m, 0, sizeof(m));
        m[0][0] = 1.0;
        m[1][1] = 1.0;
        m[2][2] = 1.0;
        m[3][3] = 1.0;
    }

    static Mat4 translation(const Vec3& v) {
        Mat4 result = identity();
        result.m[3][0] = v.x;
        result.m[3][1] = v.y;
        result.m[3][2] = v.z;
        return result;
    }

    static Mat4 scale(const Vec3& v) {
        Mat4 result = identity();
        result.m[0][0] = v.x;
        result.m[1][1] = v.y;
        result.m[2][2] = v.z;
        return result;
    }

    static Mat4 rotationZ(double angleRadians) {
        Mat4 result = identity();
        double c = std::cos(angleRadians);
        double s = std::sin(angleRadians);
        result.m[0][0] = c;  result.m[1][0] = -s;
        result.m[0][1] = s;  result.m[1][1] = c;
        return result;
    }

    static Mat4 orthographic(double left, double right, double bottom, double top, double zNear, double zFar) {
        Mat4 result = identity();
        result.m[0][0] = 2.0 / (right - left);
        result.m[1][1] = 2.0 / (top - bottom);
        result.m[2][2] = -2.0 / (zFar - zNear);
        result.m[3][0] = -(right + left) / (right - left);
        result.m[3][1] = -(top + bottom) / (top - bottom);
        result.m[3][2] = -(zFar + zNear) / (zFar - zNear);
        return result;
    }

    // Matrix Multiplication
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                result.m[col][row] = m[0][row] * other.m[col][0] +
                                    m[1][row] * other.m[col][1] +
                                    m[2][row] * other.m[col][2] +
                                    m[3][row] * other.m[col][3];
            }
        }
        return result;
    }

    Vec4 operator*(const Vec4& v) const {
        return Vec4(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
            m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w
        );
    }

    Vec3 transformPoint(const Vec3& p) const {
        Vec4 res = (*this) * Vec4(p, 1.0);
        if (std::abs(res.w) > 1e-6 && res.w != 1.0) {
            return Vec3(res.x / res.w, res.y / res.w, res.z / res.w);
        }
        return Vec3(res.x, res.y, res.z);
    }
};

} // namespace Math
} // namespace Engine

#endif // MATH_MAT4_HPP
