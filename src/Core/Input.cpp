#include "Core/Input.hpp"
#include "Debug/Logger.hpp"

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>

namespace Engine {
namespace Core {

static struct termios g_orig_termios;

Input::Input() : m_terminalModeActive(false) {}

Input::~Input() {
    shutdown();
}

void Input::init() {
    enableRawTerminalMode();
}

void Input::shutdown() {
    disableRawTerminalMode();
}

void Input::enableRawTerminalMode() {
    if (m_terminalModeActive) return;

    if (!isatty(STDIN_FILENO)) {
        // Standard input is not a TTY (e.g. redirected or piped)
        return;
    }

    if (tcgetattr(STDIN_FILENO, &g_orig_termios) == -1) {
        LOG_WARN("Failed to get terminal attributes for Input abstraction");
        return;
    }

    struct termios raw = g_orig_termios;
    // Non-canonical mode, disable echo, disable signals (like Ctrl+C handling manually if desired)
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1) {
        LOG_WARN("Failed to set raw terminal attributes for Input abstraction");
        return;
    }

    m_terminalModeActive = true;
    LOG_INFO("Input System initialized (Non-blocking terminal raw input active)");
}

void Input::disableRawTerminalMode() {
    if (!m_terminalModeActive) return;

    tcsetattr(STDIN_FILENO, TCSANOW, &g_orig_termios);
    m_terminalModeActive = false;
    LOG_INFO("Input System shutdown (Terminal mode restored)");
}

KeyCode Input::mapCharToKeyCode(char ch) const {
    switch (ch) {
        case 'q': case 'Q': case 27: return KeyCode::Quit;          // 'q' or ESC
        case 'p': case 'P': case ' ': return KeyCode::Pause;         // 'p' or Space
        case 's': case 'S':          return KeyCode::ToggleStats;   // 's'
        case '+': case '=':          return KeyCode::SpeedUp;       // '+'
        case '-': case '_':          return KeyCode::SlowDown;      // '-'
        case 'a': case 'A': case 10: return KeyCode::CustomAction;  // 'a' or Enter
        case 'h': case 'H':          return KeyCode::Help;          // 'h'
        default:                     return KeyCode::Unknown;
    }
}

void Input::pollInput() {
    m_previousKeyStates = m_keyStates;
    m_keysJustPressed.clear();
    
    // Clear momentary pressed states
    for (auto& pair : m_keyStates) {
        pair.second = false;
    }

    if (!m_terminalModeActive) return;

    // Use select to check if stdin has data available without blocking
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
        char ch;
        while (read(STDIN_FILENO, &ch, 1) > 0) {
            KeyCode key = mapCharToKeyCode(ch);
            if (key != KeyCode::Unknown) {
                m_keyStates[key] = true;
                if (!m_previousKeyStates[key]) {
                    m_keysJustPressed.push_back(key);
                }
            }
        }
    }
}

bool Input::isKeyPressed(KeyCode key) const {
    auto it = m_keyStates.find(key);
    return (it != m_keyStates.end() && it->second);
}

bool Input::wasKeyJustPressed(KeyCode key) const {
    for (KeyCode k : m_keysJustPressed) {
        if (k == key) return true;
    }
    return false;
}

} // namespace Core
} // namespace Engine
