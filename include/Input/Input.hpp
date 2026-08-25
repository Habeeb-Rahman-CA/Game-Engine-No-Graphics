#ifndef INPUT_INPUT_HPP
#define INPUT_INPUT_HPP

#include "Input/InputBackend.hpp"
#include "Input/KeyCodes.hpp"
#include "Math/Vec2.hpp"
#include <memory>

namespace Engine {
namespace InputSystem {

class Input {
public:
    static void init();
    static void shutdown();
    static void update();

    static void setBackend(std::unique_ptr<IInputBackend> backend);
    static IInputBackend* getBackend();

    // Clean static API for gameplay code
    static bool is_key_down(Key key);
    static bool is_key_pressed(Key key);
    static bool is_key_released(Key key);

    static bool is_mouse_button_down(MouseButton button);
    static bool is_mouse_button_pressed(MouseButton button);

    static Math::Vec2 mouse_position();
    static Math::Vec2 mouse_delta();

private:
    static std::unique_ptr<IInputBackend> s_backend;
};

} // namespace InputSystem
} // namespace Engine

#endif // INPUT_INPUT_HPP
