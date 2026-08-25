#ifndef INPUT_KEYCODES_HPP
#define INPUT_KEYCODES_HPP

#include <string>

namespace Engine {
namespace InputSystem {

enum class Key {
    Unknown = 0,
    
    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Numbers
    Num0, Num1, Num2, Num3, Num4,
    Num5, Num6, Num7, Num8, Num9,

    // Function & Controls
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Space,
    Enter,
    Escape,
    Tab,
    Backspace,
    Up,
    Down,
    Left,
    Right,
    
    // Engine Commands
    Plus,
    Minus,
    Equal
};

enum class MouseButton {
    Left = 0,
    Right,
    Middle
};

inline std::string keyToString(Key key) {
    switch (key) {
        case Key::A: return "A"; case Key::B: return "B"; case Key::C: return "C";
        case Key::D: return "D"; case Key::E: return "E"; case Key::F: return "F";
        case Key::G: return "G"; case Key::H: return "H"; case Key::I: return "I";
        case Key::J: return "J"; case Key::K: return "K"; case Key::L: return "L";
        case Key::M: return "M"; case Key::N: return "N"; case Key::O: return "O";
        case Key::P: return "P"; case Key::Q: return "Q"; case Key::R: return "R";
        case Key::S: return "S"; case Key::T: return "T"; case Key::U: return "U";
        case Key::V: return "V"; case Key::W: return "W"; case Key::X: return "X";
        case Key::Y: return "Y"; case Key::Z: return "Z";
        case Key::Space: return "Space";
        case Key::Enter: return "Enter";
        case Key::Escape: return "Escape";
        case Key::Up: return "Up";
        case Key::Down: return "Down";
        case Key::Left: return "Left";
        case Key::Right: return "Right";
        default: return "Key_" + std::to_string(static_cast<int>(key));
    }
}

} // namespace InputSystem
} // namespace Engine

#endif // INPUT_KEYCODES_HPP
