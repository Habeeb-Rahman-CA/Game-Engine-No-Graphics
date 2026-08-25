#include "EngineSDK.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace Engine::SDK;
using Vec2 = Engine::Math::Vec2;
using Color = Engine::RenderSystem::Color;
using AABB = Engine::Math::AABB;

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct Position2D {
    int x;
    int y;
    bool operator==(const Position2D& o) const { return x == o.x && y == o.y; }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    LOG_INFO("==========================================================================");
    LOG_INFO("          SNAKE EATER 2D - DEMO GAME POWERED BY GAME ENGINE SDK           ");
    LOG_INFO("==========================================================================");

    const int GRID_W = 24;
    const int GRID_H = 16;
    const int CELL_PX = 24; // Pixel size per grid cell

    GameEngine engine;
    if (!engine.create_window("Snake Eater 2D - Engine Showcase Demo", 1024, 640)) {
        LOG_ERROR("Failed to create graphical window for Snake Game.");
        return 1;
    }

    // Gameplay State
    std::vector<Position2D> snakeBody = { {12, 8}, {11, 8}, {10, 8} };
    Direction currentDir = Direction::Right;
    Direction nextDir = Direction::Right;
    Position2D foodPos = { 18, 8 };
    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    double moveTimer = 0.0;
    const double MOVE_INTERVAL = 0.10; // Snake updates every 100ms

    auto spawnFood = [&]() {
        bool valid = false;
        while (!valid) {
            foodPos.x = 1 + std::rand() % (GRID_W - 2);
            foodPos.y = 1 + std::rand() % (GRID_H - 2);
            valid = true;
            for (const auto& seg : snakeBody) {
                if (seg == foodPos) {
                    valid = false;
                    break;
                }
            }
        }
    };

    auto resetGame = [&]() {
        snakeBody = { {12, 8}, {11, 8}, {10, 8} };
        currentDir = Direction::Right;
        nextDir = Direction::Right;
        score = 0;
        gameOver = false;
        moveTimer = 0.0;
        spawnFood();
        LOG_INFO("Snake Game Reset! Press ARROWS or WASD to control.");
    };

    resetGame();

    // Main Game Loop using SDK callback
    engine.run([&](double dt) {
        auto window = Engine::Platform::Platform::getInstance().getWindow();
        if (!window) return;

        // 1. Input Processing for WASD and X11 Arrow Keys
        char key = window->get_last_key_char();
        uint32_t keysym = window->get_last_keysym();

        // Arrow Keys (X11 Keysyms) or WASD
        if (key == 'w' || key == 'W' || keysym == 0xFF52 || keysym == 0xFF50) {
            if (currentDir != Direction::Down) nextDir = Direction::Up;
        } else if (key == 's' || key == 'S' || keysym == 0xFF54) {
            if (currentDir != Direction::Up) nextDir = Direction::Down;
        } else if (key == 'a' || key == 'A' || keysym == 0xFF51) {
            if (currentDir != Direction::Right) nextDir = Direction::Left;
        } else if (key == 'd' || key == 'D' || keysym == 0xFF53) {
            if (currentDir != Direction::Left) nextDir = Direction::Right;
        } else if (key == 'r' || key == 'R' || key == ' ') {
            if (gameOver) resetGame();
        }

        // 2. Logic Update Tick
        if (!gameOver) {
            moveTimer += dt;
            if (moveTimer >= MOVE_INTERVAL) {
                moveTimer = 0.0;
                currentDir = nextDir;

                Position2D head = snakeBody.front();
                switch (currentDir) {
                    case Direction::Up:    head.y -= 1; break;
                    case Direction::Down:  head.y += 1; break;
                    case Direction::Left:  head.x -= 1; break;
                    case Direction::Right: head.x += 1; break;
                }

                // Wall Collision Check
                if (head.x <= 0 || head.x >= GRID_W - 1 || head.y <= 0 || head.y >= GRID_H - 1) {
                    gameOver = true;
                    if (score > highScore) highScore = score;
                    LOG_WARN("Game Over! Snake hit outer wall bounds. Final Score: " + std::to_string(score));
                }

                // Self Collision Check
                if (!gameOver) {
                    for (const auto& seg : snakeBody) {
                        if (seg == head) {
                            gameOver = true;
                            if (score > highScore) highScore = score;
                            LOG_WARN("Game Over! Snake collided with itself. Final Score: " + std::to_string(score));
                            break;
                        }
                    }
                }

                // Food Collision Check
                if (!gameOver) {
                    snakeBody.insert(snakeBody.begin(), head);

                    if (head == foodPos) {
                        score += 10;
                        if (score > highScore) highScore = score;
                        LOG_INFO("Apple Eaten! Score: " + std::to_string(score) + " | Length: " + std::to_string(snakeBody.size()));
                        spawnFood();
                    } else {
                        snakeBody.pop_back(); // Move tail forward
                    }
                }
            }
        }

        // 3. Rendering Pipeline
        window->clear_buffer(0xFF161A24); // Dark slate blue theme

        int startX = 220;
        int startY = 100;

        // Render Grid Background & Boundary Walls
        for (int y = 0; y < GRID_H; ++y) {
            for (int x = 0; x < GRID_W; ++x) {
                int px = startX + x * CELL_PX;
                int py = startY + y * CELL_PX;

                if (x == 0 || x == GRID_W - 1 || y == 0 || y == GRID_H - 1) {
                    // Wall Tiles
                    window->set_pixel(px, py, 0xFF7F8C8D);
                }
            }
        }

        // Render Snake Body
        for (size_t i = 0; i < snakeBody.size(); ++i) {
            int px = startX + snakeBody[i].x * CELL_PX;
            int py = startY + snakeBody[i].y * CELL_PX;
            uint32_t col = (i == 0) ? 0xFF2ECC71 : 0xFF27AE60; // Bright green head, darker body

            for (int dy = 1; dy < CELL_PX - 1; ++dy) {
                for (int dx = 1; dx < CELL_PX - 1; ++dx) {
                    window->set_pixel(px + dx, py + dy, col);
                }
            }
        }

        // Render Food Apple
        int fx = startX + foodPos.x * CELL_PX;
        int fy = startY + foodPos.y * CELL_PX;
        for (int dy = 2; dy < CELL_PX - 2; ++dy) {
            for (int dx = 2; dx < CELL_PX - 2; ++dx) {
                window->set_pixel(fx + dx, fy + dy, 0xFFE74C3C); // Vibrant red food
            }
        }

        window->swap_buffers();
    });

    engine.shutdown();
    return 0;
}
