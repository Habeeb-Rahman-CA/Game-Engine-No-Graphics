#ifndef STATE_GAMESTATE_HPP
#define STATE_GAMESTATE_HPP

#include <string>

namespace Engine {
namespace StateSystem {

enum class GameState {
    None,
    Menu,
    Playing,
    Paused,
    GameOver
};

inline std::string gameStateToString(GameState state) {
    switch (state) {
        case GameState::Menu:     return "Main Menu";
        case GameState::Playing:  return "Playing";
        case GameState::Paused:   return "Paused";
        case GameState::GameOver: return "Game Over";
        default:                  return "None";
    }
}

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void handleInput() = 0;
    virtual void update(double dt) = 0;
    virtual void render() = 0;
    virtual GameState getType() const = 0;
};

} // namespace StateSystem
} // namespace Engine

#endif // STATE_GAMESTATE_HPP
