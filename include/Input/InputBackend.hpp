#ifndef INPUT_INPUTBACKEND_HPP
#define INPUT_INPUTBACKEND_HPP

#include "Input/KeyCodes.hpp"
#include "Math/Vec2.hpp"

namespace Engine {
namespace InputSystem {

class IInputBackend {
public:
    virtual ~IInputBackend() = default;

    virtual void update() = 0;

    virtual bool isKeyDown(Key key) const = 0;
    virtual bool isKeyPressed(Key key) const = 0;
    virtual bool isKeyReleased(Key key) const = 0;

    virtual bool isMouseButtonDown(MouseButton button) const = 0;
    virtual bool isMouseButtonPressed(MouseButton button) const = 0;

    virtual Math::Vec2 getMousePosition() const = 0;
    virtual Math::Vec2 getMouseDelta() const = 0;
};

} // namespace InputSystem
} // namespace Engine

#endif // INPUT_INPUTBACKEND_HPP
