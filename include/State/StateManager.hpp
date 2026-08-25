#ifndef STATE_STATEMANAGER_HPP
#define STATE_STATEMANAGER_HPP

#include "State/GameState.hpp"
#include <unordered_map>
#include <memory>

namespace Engine {
namespace StateSystem {

class StateManager {
public:
    StateManager();
    ~StateManager() = default;

    void registerState(GameState type, std::unique_ptr<IGameState> state);
    void changeState(GameState newType);

    void handleInput();
    void update(double dt);
    void render();

    IGameState* getCurrentState() const { return m_currentState; }
    GameState getCurrentStateType() const { return m_currentState ? m_currentState->getType() : GameState::None; }

private:
    std::unordered_map<GameState, std::unique_ptr<IGameState>> m_states;
    IGameState* m_currentState = nullptr;
    GameState m_currentStateType = GameState::None;
};

} // namespace StateSystem
} // namespace Engine

#endif // STATE_STATEMANAGER_HPP
