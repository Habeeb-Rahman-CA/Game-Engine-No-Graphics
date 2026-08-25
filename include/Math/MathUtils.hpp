#ifndef MATH_MATHUTILS_HPP
#define MATH_MATHUTILS_HPP

#include <cmath>
#include <algorithm>

namespace Engine {
namespace Math {

constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 6.28318530717958647692;
constexpr double HALF_PI = 1.57079632679489661923;
constexpr double DEG2RAD = PI / 180.0;
constexpr double RAD2DEG = 180.0 / PI;

inline double radians(double degrees) {
    return degrees * DEG2RAD;
}

inline double degrees(double radians) {
    return radians * RAD2DEG;
}

inline double clamp(double value, double minVal, double maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

inline double lerp(double start, double end, double t) {
    return start + t * (end - start);
}

} // namespace Math
} // namespace Engine

#endif // MATH_MATHUTILS_HPP
