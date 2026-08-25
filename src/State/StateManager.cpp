#include "State/StateManager.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace StateSystem {

StateManager::StateManager() : m_currentState(nullptr), m_currentStateType(GameState::None) {}

void StateManager::registerState(GameState type, std::unique_ptr<IGameState> state) {
    m_states[type] = std::move(state);
    LOG_INFO("State Registered: " + gameStateToString(type));
}

void StateManager::changeState(GameState newType) {
    if (newType == m_currentStateType && m_currentState != nullptr) {
        return;
    }

    auto it = m_states.find(newType);
    if (it == m_states.end()) {
        LOG_ERROR("State transition failed: State '" + gameStateToString(newType) + "' not registered!");
        return;
    }

    if (m_currentState) {
        LOG_INFO("Exiting State: " + gameStateToString(m_currentStateType));
        m_currentState->onExit();
    }

    m_currentState = it->second.get();
    m_currentStateType = newType;

    LOG_INFO("Entering State: " + gameStateToString(m_currentStateType));
    m_currentState->onEnter();
}

void StateManager::handleInput() {
    if (m_currentState) {
        m_currentState->handleInput();
    }
}

void StateManager::update(double dt) {
    if (m_currentState) {
        m_currentState->update(dt);
    }
}

void StateManager::render() {
    if (m_currentState) {
        m_currentState->render();
    }
}

} // namespace StateSystem
} // namespace Engine
