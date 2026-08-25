#ifndef INPUT_TERMINALINPUTBACKEND_HPP
#define INPUT_TERMINALINPUTBACKEND_HPP

#include "Input/InputBackend.hpp"
#include <unordered_map>
#include <termios.h>

namespace Engine {
namespace InputSystem {

class TerminalInputBackend : public IInputBackend {
public:
    TerminalInputBackend();
    ~TerminalInputBackend() override;

    void update() override;

    bool isKeyDown(Key key) const override;
    bool isKeyPressed(Key key) const override;
    bool isKeyReleased(Key key) const override;

    bool isMouseButtonDown(MouseButton button) const override;
    bool isMouseButtonPressed(MouseButton button) const override;

    Math::Vec2 getMousePosition() const override { return m_mousePos; }
    Math::Vec2 getMouseDelta() const override { return m_mouseDelta; }

    void simulateMousePosition(double x, double y);
    void simulateMouseButton(MouseButton button, bool down);

private:
    bool m_rawModeActive = false;
    struct termios m_originalTermios;

    std::unordered_map<Key, bool> m_currentKeys;
    std::unordered_map<Key, bool> m_previousKeys;

    std::unordered_map<MouseButton, bool> m_currentMouseButtons;
    std::unordered_map<MouseButton, bool> m_previousMouseButtons;

    Math::Vec2 m_mousePos{0.0, 0.0};
    Math::Vec2 m_previousMousePos{0.0, 0.0};
    Math::Vec2 m_mouseDelta{0.0, 0.0};

    void enableRawMode();
    void disableRawMode();
    Key charToKey(char ch) const;
};

} // namespace InputSystem
} // namespace Engine

#endif // INPUT_TERMINALINPUTBACKEND_HPP
