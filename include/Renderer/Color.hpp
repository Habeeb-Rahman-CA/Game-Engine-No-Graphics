#ifndef RENDERER_COLOR_HPP
#define RENDERER_COLOR_HPP

#include <cstdint>

namespace Engine {
namespace RenderSystem {

struct Color {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    Color() = default;
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Black;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Clear;
};

inline const Color Color::Red(255, 0, 0);
inline const Color Color::Green(0, 255, 0);
inline const Color Color::Blue(0, 0, 255);
inline const Color Color::White(255, 255, 255);
inline const Color Color::Black(0, 0, 0);
inline const Color Color::Yellow(255, 255, 0);
inline const Color Color::Cyan(0, 255, 255);
inline const Color Color::Magenta(255, 0, 255);
inline const Color Color::Clear(0, 0, 0, 0);

} // namespace RenderSystem
} // namespace Engine

#endif // RENDERER_COLOR_HPP
