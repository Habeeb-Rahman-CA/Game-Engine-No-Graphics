#include "Input/Input.hpp"
#include "Input/TerminalInputBackend.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace InputSystem {

std::unique_ptr<IInputBackend> Input::s_backend = nullptr;

void Input::init() {
    if (!s_backend) {
        s_backend = std::make_unique<TerminalInputBackend>();
        LOG_INFO("Engine Input System Facade initialized with default Terminal backend.");
    }
}

void Input::shutdown() {
    if (s_backend) {
        s_backend.reset();
        LOG_INFO("Engine Input System Facade shutdown.");
    }
}

void Input::setBackend(std::unique_ptr<IInputBackend> backend) {
    s_backend = std::move(backend);
}

IInputBackend* Input::getBackend() {
    return s_backend.get();
}

void Input::update() {
    if (s_backend) {
        s_backend->update();
    }
}

bool Input::is_key_down(Key key) {
    return s_backend ? s_backend->isKeyDown(key) : false;
}

bool Input::is_key_pressed(Key key) {
    return s_backend ? s_backend->isKeyPressed(key) : false;
}

bool Input::is_key_released(Key key) {
    return s_backend ? s_backend->isKeyReleased(key) : false;
}

bool Input::is_mouse_button_down(MouseButton button) {
    return s_backend ? s_backend->isMouseButtonDown(button) : false;
}

bool Input::is_mouse_button_pressed(MouseButton button) {
    return s_backend ? s_backend->isMouseButtonPressed(button) : false;
}

Math::Vec2 Input::mouse_position() {
    return s_backend ? s_backend->getMousePosition() : Math::Vec2(0.0, 0.0);
}

Math::Vec2 Input::mouse_delta() {
    return s_backend ? s_backend->getMouseDelta() : Math::Vec2(0.0, 0.0);
}

} // namespace InputSystem
} // namespace Engine
