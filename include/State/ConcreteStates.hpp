#ifndef STATE_CONCRETESTATES_HPP
#define STATE_CONCRETESTATES_HPP

#include "State/GameState.hpp"
#include "State/StateManager.hpp"

namespace Engine {
namespace StateSystem {

class MenuState : public IGameState {
public:
    explicit MenuState(StateManager& manager) : m_manager(manager) {}
    ~MenuState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput() override;
    void update(double dt) override;
    void render() override;
    GameState getType() const override { return GameState::Menu; }

private:
    StateManager& m_manager;
};

class PlayingState : public IGameState {
public:
    explicit PlayingState(StateManager& manager) : m_manager(manager), m_score(0) {}
    ~PlayingState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput() override;
    void update(double dt) override;
    void render() override;
    GameState getType() const override { return GameState::Playing; }

    int getScore() const { return m_score; }

private:
    StateManager& m_manager;
    int m_score = 0;
    double m_elapsed = 0.0;
};

class PausedState : public IGameState {
public:
    explicit PausedState(StateManager& manager) : m_manager(manager) {}
    ~PausedState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput() override;
    void update(double dt) override;
    void render() override;
    GameState getType() const override { return GameState::Paused; }

private:
    StateManager& m_manager;
};

class GameOverState : public IGameState {
public:
    explicit GameOverState(StateManager& manager) : m_manager(manager) {}
    ~GameOverState() override = default;

    void onEnter() override;
    void onExit() override;
    void handleInput() override;
    void update(double dt) override;
    void render() override;
    GameState getType() const override { return GameState::GameOver; }

private:
    StateManager& m_manager;
};

} // namespace StateSystem
} // namespace Engine

#endif // STATE_CONCRETESTATES_HPP
