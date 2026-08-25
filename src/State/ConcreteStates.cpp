#include "State/ConcreteStates.hpp"
#include "Input/Input.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace StateSystem {

// ============================================================================
// MENU STATE
// ============================================================================
void MenuState::onEnter() {
    LOG_INFO(">>> MAIN MENU: Press [ENTER] or [SPACE] to Start Game | [Q] to Quit <<<");
}

void MenuState::onExit() {
    LOG_INFO("<<< Leaving Main Menu...");
}

void MenuState::handleInput() {
    using namespace InputSystem;
    if (Input::is_key_pressed(Key::Enter) || Input::is_key_pressed(Key::Space)) {
        LOG_INFO("Start Game requested from Main Menu.");
        m_manager.changeState(GameState::Playing);
    }
}

void MenuState::update(double dt) {
    (void)dt;
}

void MenuState::render() {
    // Menu render frame
}

// ============================================================================
// PLAYING STATE
// ============================================================================
void PlayingState::onEnter() {
    LOG_INFO(">>> GAMEPLAY ACTIVE: Press [P] to Pause | Simulated combat active <<<");
    m_score = 0;
    m_elapsed = 0.0;
}

void PlayingState::onExit() {
    LOG_INFO("<<< Leaving Playing State...");
}

void PlayingState::handleInput() {
    using namespace InputSystem;
    if (Input::is_key_pressed(Key::P)) {
        LOG_INFO("Pause requested during Gameplay.");
        m_manager.changeState(GameState::Paused);
    }
}

void PlayingState::update(double dt) {
    m_elapsed += dt;
    m_score += static_cast<int>(dt * 100.0);

    // Simulate Game Over condition after 3 seconds of gameplay for state demo
    if (m_elapsed >= 3.0) {
        LOG_INFO("Player health reached 0! Transitioning to Game Over state.");
        m_manager.changeState(GameState::GameOver);
    }
}

void PlayingState::render() {
    // Gameplay render frame
}

// ============================================================================
// PAUSED STATE
// ============================================================================
void PausedState::onEnter() {
    LOG_INFO(">>> GAME PAUSED: Press [P] to Resume | [Q] for Main Menu <<<");
}

void PausedState::onExit() {
    LOG_INFO("<<< Leaving Paused State...");
}

void PausedState::handleInput() {
    using namespace InputSystem;
    if (Input::is_key_pressed(Key::P) || Input::is_key_pressed(Key::Space)) {
        LOG_INFO("Resuming Game from Pause...");
        m_manager.changeState(GameState::Playing);
    } else if (Input::is_key_pressed(Key::Q)) {
        LOG_INFO("Returning to Main Menu from Pause...");
        m_manager.changeState(GameState::Menu);
    }
}

void PausedState::update(double dt) {
    (void)dt; // No gameplay logic updates while paused
}

void PausedState::render() {
    // Pause screen render overlay
}

// ============================================================================
// GAME OVER STATE
// ============================================================================
void GameOverState::onEnter() {
    LOG_INFO(">>> GAME OVER: Press [R] / [ENTER] to Restart | [Q] for Main Menu <<<");
}

void GameOverState::onExit() {
    LOG_INFO("<<< Leaving Game Over State...");
}

void GameOverState::handleInput() {
    using namespace InputSystem;
    if (Input::is_key_pressed(Key::R) || Input::is_key_pressed(Key::Enter) || Input::is_key_pressed(Key::Space)) {
        LOG_INFO("Restarting Game from Game Over screen...");
        m_manager.changeState(GameState::Playing);
    } else if (Input::is_key_pressed(Key::Q)) {
        LOG_INFO("Returning to Main Menu from Game Over screen...");
        m_manager.changeState(GameState::Menu);
    }
}

void GameOverState::update(double dt) {
    (void)dt;
}

void GameOverState::render() {
    // Game over screen render overlay
}

} // namespace StateSystem
} // namespace Engine
