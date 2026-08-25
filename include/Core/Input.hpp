#ifndef CORE_INPUT_HPP
#define CORE_INPUT_HPP

#include <unordered_map>
#include <vector>
#include <string>

namespace Engine {
namespace Core {

enum class KeyCode {
    Unknown,
    Quit,          // 'q' or ESC
    Pause,         // 'p' or Space
    ToggleStats,   // 's'
    SpeedUp,       // '+' or '='
    SlowDown,      // '-'
    CustomAction,  // 'a' or Enter
    Help           // 'h'
};

inline std::string keyCodeToString(KeyCode key) {
    switch (key) {
        case KeyCode::Quit:         return "Quit";
        case KeyCode::Pause:        return "Pause / Resume";
        case KeyCode::ToggleStats:  return "Toggle Statistics";
        case KeyCode::SpeedUp:      return "Increase Speed";
        case KeyCode::SlowDown:     return "Decrease Speed";
        case KeyCode::CustomAction: return "Custom Action";
        case KeyCode::Help:         return "Help";
        default:                    return "Unknown";
    }
}

class Input {
public:
    Input();
    ~Input();

    void init();
    void shutdown();
    void pollInput();

    bool isKeyPressed(KeyCode key) const;
    bool wasKeyJustPressed(KeyCode key) const;
    
    const std::vector<KeyCode>& getPressedThisFrame() const { return m_keysJustPressed; }

private:
    bool m_terminalModeActive;
    std::unordered_map<KeyCode, bool> m_keyStates;
    std::unordered_map<KeyCode, bool> m_previousKeyStates;
    std::vector<KeyCode> m_keysJustPressed;

    void enableRawTerminalMode();
    void disableRawTerminalMode();
    KeyCode mapCharToKeyCode(char ch) const;
};

} // namespace Core
} // namespace Engine

#endif // CORE_INPUT_HPP
