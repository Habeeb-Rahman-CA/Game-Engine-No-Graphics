#include "Input/TerminalInputBackend.hpp"
#include "Debug/Logger.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <iostream>

namespace Engine {
namespace InputSystem {

TerminalInputBackend::TerminalInputBackend() {
    enableRawMode();
}

TerminalInputBackend::~TerminalInputBackend() {
    disableRawMode();
}

void TerminalInputBackend::enableRawMode() {
    if (m_rawModeActive) return;
    if (tcgetattr(STDIN_FILENO, &m_originalTermios) == -1) return;

    struct termios raw = m_originalTermios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0) {
        m_rawModeActive = true;
        LOG_INFO("Terminal Input Backend initialized (Raw Non-blocking mode active)");
    }
}

void TerminalInputBackend::disableRawMode() {
    if (!m_rawModeActive) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_originalTermios);
    m_rawModeActive = false;
    LOG_INFO("Terminal Input Backend shutdown (Terminal settings restored)");
}

Key TerminalInputBackend::charToKey(char ch) const {
    switch (ch) {
        case 'w': case 'W': return Key::W;
        case 'a': case 'A': return Key::A;
        case 's': case 'S': return Key::S;
        case 'd': case 'D': return Key::D;
        case 'q': case 'Q': return Key::Q;
        case 'e': case 'E': return Key::E;
        case 'p': case 'P': return Key::P;
        case 'h': case 'H': return Key::H;
        case ' ':           return Key::Space;
        case 27:            return Key::Escape;
        case 10: case 13:   return Key::Enter;
        case '+': case '=': return Key::Plus;
        case '-':           return Key::Minus;
        default:
            if (ch >= 'a' && ch <= 'z') return static_cast<Key>(static_cast<int>(Key::A) + (ch - 'a'));
            if (ch >= 'A' && ch <= 'Z') return static_cast<Key>(static_cast<int>(Key::A) + (ch - 'A'));
            return Key::Unknown;
    }
}

void TerminalInputBackend::update() {
    m_previousKeys = m_currentKeys;
    m_previousMouseButtons = m_currentMouseButtons;

    // Reset instant key presses for single poll cycle
    m_currentKeys.clear();

    m_mouseDelta = m_mousePos - m_previousMousePos;
    m_previousMousePos = m_mousePos;

    if (!m_rawModeActive) return;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    struct timeval tv = {0, 0};
    int selectResult = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);

    if (selectResult > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
        char ch = 0;
        while (read(STDIN_FILENO, &ch, 1) > 0) {
            Key key = charToKey(ch);
            if (key != Key::Unknown) {
                m_currentKeys[key] = true;
            }
        }
    }
}

bool TerminalInputBackend::isKeyDown(Key key) const {
    auto it = m_currentKeys.find(key);
    return (it != m_currentKeys.end()) && it->second;
}

bool TerminalInputBackend::isKeyPressed(Key key) const {
    bool current = isKeyDown(key);
    auto itPrev = m_previousKeys.find(key);
    bool previous = (itPrev != m_previousKeys.end()) && itPrev->second;

    return current && !previous;
}

bool TerminalInputBackend::isKeyReleased(Key key) const {
    bool current = isKeyDown(key);
    auto itPrev = m_previousKeys.find(key);
    bool previous = (itPrev != m_previousKeys.end()) && itPrev->second;

    return !current && previous;
}

bool TerminalInputBackend::isMouseButtonDown(MouseButton button) const {
    auto it = m_currentMouseButtons.find(button);
    return (it != m_currentMouseButtons.end()) && it->second;
}

bool TerminalInputBackend::isMouseButtonPressed(MouseButton button) const {
    bool current = isMouseButtonDown(button);
    auto itPrev = m_previousMouseButtons.find(button);
    bool previous = (itPrev != m_previousMouseButtons.end()) && itPrev->second;

    return current && !previous;
}

void TerminalInputBackend::simulateMousePosition(double x, double y) {
    m_mousePos = Math::Vec2(x, y);
}

void TerminalInputBackend::simulateMouseButton(MouseButton button, bool down) {
    m_currentMouseButtons[button] = down;
}

} // namespace InputSystem
} // namespace Engine
