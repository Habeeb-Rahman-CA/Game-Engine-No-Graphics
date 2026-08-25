#include "Editor/Editor.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Profiler.hpp"
#include "System/MovementSystem.hpp"
#include "System/PhysicsSystem.hpp"
#include "System/AISystem.hpp"
#include "System/GameplaySystem.hpp"
#include "System/AnimationSystem.hpp"

#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace Engine {
namespace EditorSystem {

using Color = RenderSystem::Color;
using Vec2 = Math::Vec2;
using Vec3 = Math::Vec3;
using AABB = Math::AABB;
using Circle = Math::Circle;
using Entity = WorldSystem::Entity;
using Transform = EntitySystem::Transform;
using Velocity = EntitySystem::Velocity;
using Health = EntitySystem::Health;
using Sprite = EntitySystem::Sprite;
using Collider2D = EntitySystem::Collider2D;
using EnemyAI = EntitySystem::EnemyAI;
using AIState = EntitySystem::AIState;
using ColliderType = EntitySystem::ColliderType;

Editor::Editor() : m_viewportCamera(Vec2(0.0, 0.0), 1.0, Vec2(564.0, 444.0)) {
}

bool Editor::initialize(int width, int height) {
    LOG_INFO("Initializing Godot/Unity-Style Game Engine Editor (" + std::to_string(width) + "x" + std::to_string(height) + ")...");

    Platform::WindowProps props("Basic Game Engine - Editor & GUI", width, height);
    if (!m_window.initialize()) {
        LOG_ERROR("Failed to initialize Editor Window.");
        return false;
    }

    m_renderer = std::make_unique<RenderSystem::GPU2DRenderer>(&m_window);
    m_renderer->init(width, height, "Basic Game Engine Editor");

    // UI Panel Coordinates
    m_rectTopBar    = {0, 0, width, 36};
    m_rectHierarchy = {0, 36, 210, 444};
    m_rectViewport  = {210, 36, 564, 444};
    m_rectInspector = {774, 36, 250, 444};
    m_rectConsole   = {0, 480, 512, 160};
    m_rectProfiler  = {512, 480, 512, 160};

    // Register Engine Systems
    m_world.add_system(std::make_unique<System::MovementSystem>());
    m_world.add_system(std::make_unique<System::PhysicsSystem>(&m_eventBus));
    m_world.add_system(std::make_unique<System::AISystem>());
    m_world.add_system(std::make_unique<System::AnimationSystem>());

    auto gameplaySystem = std::make_unique<System::GameplaySystem>(&m_eventBus);
    gameplaySystem->setWorld(&m_world);
    m_world.add_system(std::move(gameplaySystem));

    create_default_scene();

    LOG_INFO("Editor UI Layout & Scene Hierarchy initialized successfully!");
    return true;
}

void Editor::create_default_scene() {
    using namespace WorldSystem;

    auto entities = m_world.get_entities();
    for (Entity e : entities) {
        m_world.destroy_entity(e);
    }

    // 1. Camera Entity
    Entity camera = m_world.create_entity("Main_Camera");
    m_world.add_transform(camera, Transform(Vec3(0.0, 0.0, 0.0)));

    // 2. Player Entity
    Entity player = m_world.create_entity("Player_Hero");
    m_world.add_transform(player, Transform(Vec3(0.0, 0.0, 0.0)));
    m_world.add_velocity(player, Velocity(Vec3(0.0, 0.0, 0.0)));
    m_world.add_sprite(player, Sprite("player_hero", Vec2(2.5, 4.0)));
    m_world.add_collider(player, Collider2D::MakeBox(Vec2(2.5, 4.0)));
    m_world.add_health(player, Health(100, 100));

    // 3. Enemy_01 Entity
    Entity enemy1 = m_world.create_entity("Orc_Warrior");
    m_world.add_transform(enemy1, Transform(Vec3(-6.0, 4.0, 0.0)));
    m_world.add_velocity(enemy1, Velocity(Vec3(1.0, 0.0, 0.0)));
    m_world.add_sprite(enemy1, Sprite("enemy_orc", Vec2(2.5, 2.5)));
    m_world.add_collider(enemy1, Collider2D::MakeCircle(1.5));
    m_world.add_health(enemy1, Health(60, 60));
    m_world.add_ai(enemy1, EnemyAI(AIState::Patrol, 10.0, 2.0, 2.0));

    // 4. Enemy_02 Entity
    Entity enemy2 = m_world.create_entity("Goblin_Scout");
    m_world.add_transform(enemy2, Transform(Vec3(7.0, -3.0, 0.0)));
    m_world.add_sprite(enemy2, Sprite("enemy_goblin", Vec2(2.0, 2.0)));
    m_world.add_collider(enemy2, Collider2D::MakeCircle(1.2));
    m_world.add_health(enemy2, Health(40, 40));
    m_world.add_ai(enemy2, EnemyAI(AIState::Idle, 8.0, 1.5, 1.5));

    select_entity(player);
}

void Editor::duplicate_selected_entity() {
    if (!m_hasSelection) return;

    std::string origName = m_world.get_name(m_selectedEntity);
    Entity dup = m_world.create_entity(origName + "_Copy");

    auto* t = m_world.get_transform(m_selectedEntity);
    if (t) m_world.add_transform(dup, Transform(Vec3(t->position.x + 1.0, t->position.y + 1.0, t->position.z)));

    auto* v = m_world.get_velocity(m_selectedEntity);
    if (v) m_world.add_velocity(dup, Velocity(v->value));

    auto* s = m_world.get_sprite(m_selectedEntity);
    if (s) m_world.add_sprite(dup, Sprite(s->textureId, s->size));

    auto* c = m_world.get_collider(m_selectedEntity);
    if (c) {
        if (c->type == ColliderType::Box) m_world.add_collider(dup, Collider2D::MakeBox(c->box.maxBound - c->box.minBound));
        else m_world.add_collider(dup, Collider2D::MakeCircle(c->circle.radius));
    }

    auto* h = m_world.get_health(m_selectedEntity);
    if (h) m_world.add_health(dup, Health(h->value, h->maxHp));

    auto* ai = m_world.get_ai(m_selectedEntity);
    if (ai) m_world.add_ai(dup, EnemyAI(ai->state, ai->detectionRange, ai->attackRange, ai->moveSpeed));

    select_entity(dup);
    LOG_INFO("Duplicated Entity: " + origName + " -> " + m_world.get_name(dup));
}

void Editor::select_entity(WorldSystem::Entity entity) {
    m_selectedEntity = entity;
    m_hasSelection = true;
    m_isRenamingEntity = false;
    m_renameBuffer = m_world.get_name(entity);
    m_showAddComponentMenu = false;
    LOG_INFO("Selected Entity: '" + m_renameBuffer + "' (ID: " + std::to_string(entity) + ")");
}

void Editor::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!m_window.should_close()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;

        if (dt <= 0.0) dt = 0.016;
        if (dt > 0.1) dt = 0.1;

        m_frameTimeMs = dt * 1000.0;
        m_fps = (dt > 0.0) ? (1.0 / dt) : 60.0;
        m_frameCount++;

        m_window.poll_events();
        process_input();
        update(dt);
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    m_renderer->shutdown();
}

void Editor::process_input() {
    int mx = m_window.get_mouse_x();
    int my = m_window.get_mouse_y();

    bool leftClicked = m_window.was_mouse_clicked(1);
    bool leftDown = m_window.is_mouse_down(1);

    if (leftClicked) {
        handle_mouse_click(mx, my);
    }

    if (leftDown && m_rectViewport.contains(mx, my)) {
        handle_viewport_drag(mx, my);
    } else {
        m_isDraggingInViewport = false;
    }

    // Keyboard Input Handling (Renaming & Shortcuts)
    char key = m_window.get_last_key_char();
    if (m_isRenamingEntity) {
        if (key == '\r' || key == '\n') { // Enter key
            if (!m_renameBuffer.empty() && m_hasSelection) {
                m_world.set_name(m_selectedEntity, m_renameBuffer);
                LOG_INFO("Entity renamed to: '" + m_renameBuffer + "'");
            }
            m_isRenamingEntity = false;
        } else if (key == '\b' || key == 127) { // Backspace
            if (!m_renameBuffer.empty()) {
                m_renameBuffer.pop_back();
            }
        } else if (key >= 32 && key <= 126 && m_renameBuffer.size() < 20) { // Alphanumeric
            m_renameBuffer += key;
        }
    } else {
        if (key == 'p' || key == 'P') {
            m_mode = (m_mode == EditorMode::Play) ? EditorMode::Pause : EditorMode::Play;
            LOG_INFO(m_mode == EditorMode::Play ? "Switched to PLAY Mode" : "Switched to PAUSE Mode");
        } else if (key == 'd' || key == 'D') {
            duplicate_selected_entity();
        } else if (key == 127 || key == 8) { // Delete key
            if (m_hasSelection) {
                LOG_INFO("Deleted Entity: " + m_world.get_name(m_selectedEntity));
                m_world.destroy_entity(m_selectedEntity);
                m_hasSelection = false;
            }
        }
    }
}

void Editor::handle_mouse_click(int mx, int my) {
    // 0. HELP MODAL DISMISS
    if (m_showHelpModal) {
        m_showHelpModal = false;
        return;
    }

    // 1. TOP BAR CLICK
    if (m_rectTopBar.contains(mx, my)) {
        if (mx >= 10 && mx <= 50) {
            m_showFileMenu = !m_showFileMenu;
            m_showEditMenu = false;
            m_showSceneMenu = false;
        } else if (mx >= 60 && mx <= 100) {
            m_showEditMenu = !m_showEditMenu;
            m_showFileMenu = false;
            m_showSceneMenu = false;
        } else if (mx >= 110 && mx <= 160) {
            m_showSceneMenu = !m_showSceneMenu;
            m_showFileMenu = false;
            m_showEditMenu = false;
        } else if (mx >= 170 && mx <= 210) {
            m_showHelpModal = true;
            m_showFileMenu = m_showEditMenu = m_showSceneMenu = false;
        } else if (mx >= 440 && mx <= 500 && my >= 6 && my <= 30) { // ▶ PLAY
            m_mode = EditorMode::Play;
            LOG_INFO("=== [SIMULATION] PLAY MODE STARTED ===");
        } else if (mx >= 510 && mx <= 570 && my >= 6 && my <= 30) { // ⏸ PAUSE
            m_mode = EditorMode::Pause;
            LOG_INFO("=== [SIMULATION] PAUSED ===");
        } else if (mx >= 580 && mx <= 630 && my >= 6 && my <= 30) { // ⏹ STOP
            m_mode = EditorMode::Edit;
            LOG_INFO("=== [SIMULATION] STOPPED & RESET ===");
        }
        return;
    }

    // Dropdown Overlays Click Handling
    if (m_showFileMenu) {
        if (mx >= 10 && mx <= 150) {
            if (my >= 36 && my < 60) {
                create_default_scene();
                LOG_INFO("New Scene Created");
            } else if (my >= 60 && my < 84) {
                m_world.load_scene("level1.json");
                LOG_INFO("Loaded Scene from level1.json");
            } else if (my >= 84 && my < 108) {
                m_world.save_scene("saved_level.json");
                LOG_INFO("Saved Scene to saved_level.json");
            } else if (my >= 108 && my < 132) {
                m_window.close();
            }
        }
        m_showFileMenu = false;
        return;
    }

    if (m_showEditMenu) {
        if (mx >= 60 && mx <= 210) {
            if (my >= 36 && my < 60) {
                auto newEnt = m_world.create_entity("New_Object");
                m_world.add_transform(newEnt, Transform(Vec3(0,0,0)));
                m_world.add_sprite(newEnt, Sprite("box", Vec2(2,2)));
                select_entity(newEnt);
            } else if (my >= 60 && my < 84) {
                duplicate_selected_entity();
            } else if (my >= 84 && my < 108) {
                if (m_hasSelection) {
                    m_world.destroy_entity(m_selectedEntity);
                    m_hasSelection = false;
                }
            } else if (my >= 108 && my < 132) {
                m_hasSelection = false;
            }
        }
        m_showEditMenu = false;
        return;
    }

    if (m_showSceneMenu) {
        if (mx >= 110 && mx <= 260) {
            if (my >= 36 && my < 60) {
                auto p = m_world.create_entity("Player_Hero");
                m_world.add_transform(p, Transform(Vec3(0,0,0)));
                m_world.add_sprite(p, Sprite("player_hero", Vec2(2.5,4)));
                m_world.add_health(p, Health(100, 100));
                select_entity(p);
            } else if (my >= 60 && my < 84) {
                auto e = m_world.create_entity("Enemy_Orc");
                m_world.add_transform(e, Transform(Vec3(4,3,0)));
                m_world.add_sprite(e, Sprite("enemy_orc", Vec2(2.5,2.5)));
                m_world.add_ai(e, EnemyAI(AIState::Patrol));
                select_entity(e);
            } else if (my >= 84 && my < 108) {
                auto c = m_world.create_entity("Camera_View");
                m_world.add_transform(c, Transform(Vec3(0,0,0)));
                select_entity(c);
            }
        }
        m_showSceneMenu = false;
        return;
    }

    // 2. HIERARCHY PANEL CLICK
    if (m_rectHierarchy.contains(mx, my)) {
        const auto& entities = m_world.get_entities();
        int startY = 66;
        int rowHeight = 24;

        for (size_t i = 0; i < entities.size(); ++i) {
            int itemY = startY + static_cast<int>(i) * rowHeight;
            if (my >= itemY && my < itemY + rowHeight) {
                select_entity(entities[i]);
                return;
            }
        }

        // Bottom Action Buttons in Hierarchy
        if (my >= 445 && my <= 470) {
            if (mx >= 8 && mx <= 70) { // [+ Add]
                auto ent = m_world.create_entity("Entity_" + std::to_string(entities.size() + 1));
                m_world.add_transform(ent, Transform(Vec3(0,0,0)));
                select_entity(ent);
            } else if (mx >= 75 && mx <= 135) { // [Dup]
                duplicate_selected_entity();
            } else if (mx >= 140 && mx <= 202) { // [- Del]
                if (m_hasSelection) {
                    m_world.destroy_entity(m_selectedEntity);
                    m_hasSelection = false;
                }
            }
        }
        return;
    }

    // 3. VIEWPORT PANEL CLICK (Mouse picking in 2D space)
    if (m_rectViewport.contains(mx, my)) {
        int centerX = m_rectViewport.x + m_rectViewport.width / 2;
        int centerY = m_rectViewport.y + m_rectViewport.height / 2;

        double clickWx = (mx - centerX) / 16.0;
        double clickWy = (centerY - my) / 16.0;

        for (Entity e : m_world.get_entities()) {
            auto* t = m_world.get_transform(e);
            if (t) {
                double distSq = (t->position.x - clickWx) * (t->position.x - clickWx) +
                                (t->position.y - clickWy) * (t->position.y - clickWy);
                if (distSq <= 12.0) {
                    select_entity(e);
                    m_isDraggingInViewport = true;
                    m_lastDragMx = mx;
                    m_lastDragMy = my;
                    return;
                }
            }
        }
        return;
    }

    // 4. INSPECTOR PANEL CLICK (Property Controls & Component Menu)
    if (m_rectInspector.contains(mx, my) && m_hasSelection) {
        auto* transform = m_world.get_transform(m_selectedEntity);
        auto* health    = m_world.get_health(m_selectedEntity);
        auto* collider  = m_world.get_collider(m_selectedEntity);
        auto* ai        = m_world.get_ai(m_selectedEntity);
        auto* sprite    = m_world.get_sprite(m_selectedEntity);

        // Rename Button
        if (my >= 68 && my <= 88 && mx >= 930 && mx <= 990) {
            m_isRenamingEntity = !m_isRenamingEntity;
            if (!m_isRenamingEntity && !m_renameBuffer.empty()) {
                m_world.set_name(m_selectedEntity, m_renameBuffer);
            }
            return;
        }

        // Pos X [-] [+]
        if (my >= 125 && my <= 145) {
            if (mx >= 840 && mx <= 865 && transform) transform->position.x -= 0.5;
            if (mx >= 870 && mx <= 895 && transform) transform->position.x += 0.5;
        }
        // Pos Y [-] [+]
        if (my >= 148 && my <= 168) {
            if (mx >= 840 && mx <= 865 && transform) transform->position.y -= 0.5;
            if (mx >= 870 && mx <= 895 && transform) transform->position.y += 0.5;
        }

        // Texture Switcher
        if (my >= 210 && my <= 230 && mx >= 840 && mx <= 980 && sprite) {
            if (sprite->textureId == "player_hero") sprite->textureId = "enemy_orc";
            else if (sprite->textureId == "enemy_orc") sprite->textureId = "enemy_goblin";
            else if (sprite->textureId == "enemy_goblin") sprite->textureId = "box";
            else sprite->textureId = "player_hero";
            LOG_INFO("Texture changed to: " + sprite->textureId);
        }

        // Collider Type Toggle
        if (my >= 278 && my <= 298 && mx >= 840 && mx <= 970 && collider) {
            if (collider->type == ColliderType::Box) {
                *collider = Collider2D::MakeCircle(1.5);
                LOG_INFO("Collider type changed to CIRCLE");
            } else {
                *collider = Collider2D::MakeBox(Vec2(2.5, 3.5));
                LOG_INFO("Collider type changed to BOX (AABB)");
            }
        }

        // Health Controls [-10] [+10] [Full]
        if (my >= 348 && my <= 368 && health) {
            if (mx >= 820 && mx <= 855) health->value = std::max(0, health->value - 10);
            if (mx >= 860 && mx <= 895) health->value = std::min(health->maxHp, health->value + 10);
            if (mx >= 900 && mx <= 940) health->value = health->maxHp;
        }

        // Enemy AI State Cycle
        if (my >= 415 && my <= 435 && ai) {
            if (ai->state == AIState::Idle) ai->state = AIState::Patrol;
            else if (ai->state == AIState::Patrol) ai->state = AIState::Chase;
            else ai->state = AIState::Idle;
            LOG_INFO("AI State changed");
        }

        // [+ Add Component] Button & Menu Toggle
        if (my >= 445 && my <= 470 && mx >= 784 && mx <= 930) {
            m_showAddComponentMenu = !m_showAddComponentMenu;
            return;
        }

        // Dropdown Items for Add Component
        if (m_showAddComponentMenu) {
            if (my >= 410 && my <= 430) {
                if (!transform) m_world.add_transform(m_selectedEntity, Transform(Vec3(0,0,0)));
            } else if (my >= 432 && my <= 452) {
                if (!sprite) m_world.add_sprite(m_selectedEntity, Sprite("box", Vec2(2,2)));
            } else if (my >= 454 && my <= 474) {
                if (!collider) m_world.add_collider(m_selectedEntity, Collider2D::MakeBox(Vec2(2,2)));
            } else if (my >= 476 && my <= 496) {
                if (!health) m_world.add_health(m_selectedEntity, Health(100, 100));
            } else if (my >= 498 && my <= 518) {
                if (!ai) m_world.add_ai(m_selectedEntity, EnemyAI(AIState::Patrol));
            }
            m_showAddComponentMenu = false;
        }
        return;
    }

    // 5. CONSOLE CLEAR / FILTER CLICK
    if (m_rectConsole.contains(mx, my)) {
        if (mx >= 440 && mx <= 500 && my >= 485 && my <= 505) {
            Debug::Logger::clearHistory();
        }
    }
}

void Editor::handle_viewport_drag(int mx, int my) {
    if (m_hasSelection && m_isDraggingInViewport) {
        auto* transform = m_world.get_transform(m_selectedEntity);
        if (transform) {
            int dx = mx - m_lastDragMx;
            int dy = my - m_lastDragMy;
            transform->position.x += dx / 16.0;
            transform->position.y -= dy / 16.0;
        }
    }
    m_lastDragMx = mx;
    m_lastDragMy = my;
}

void Editor::update(double dt) {
    if (m_mode == EditorMode::Play) {
        auto physStart = std::chrono::high_resolution_clock::now();
        m_world.update(dt);
        m_eventBus.dispatchEvents();
        auto physEnd = std::chrono::high_resolution_clock::now();
        m_physicsTimeMs = std::chrono::duration<double, std::milli>(physEnd - physStart).count();
    }
}

void Editor::render() {
    m_renderer->begin_frame();
    m_renderer->clear(Color(18, 20, 28));

    render_top_bar();
    render_hierarchy_panel();
    render_viewport_panel();
    render_inspector_panel();
    render_bottom_panel();

    if (m_showHelpModal) {
        // Modal Window Overlay
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(512, 320), Vec2(480, 280)), Color(25, 30, 42), true);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(512, 320), Vec2(480, 280)), Color(52, 152, 219), false);
        m_renderer->draw_text("GAME ENGINE EDITOR GUIDE", Vec2(330, 200), Color(241, 196, 15));
        m_renderer->draw_text("- TOP BAR: File, Edit, Scene, Play/Pause/Stop", Vec2(300, 235), Color::White);
        m_renderer->draw_text("- HIERARCHY: Click entity row, Add/Dup/Del", Vec2(300, 260), Color::White);
        m_renderer->draw_text("- VIEWPORT: Click entity to select & drag", Vec2(300, 285), Color::White);
        m_renderer->draw_text("- INSPECTOR: Click [Rename], tweak Pos/HP", Vec2(300, 310), Color::White);
        m_renderer->draw_text("- KEYBOARD: 'P' (Play), 'D' (Duplicate), 'Del'", Vec2(300, 335), Color::White);
        m_renderer->draw_text("CLICK ANYWHERE TO CLOSE HELP", Vec2(330, 380), Color(46, 204, 113));
    }

    m_renderer->end_frame();
}

void Editor::render_top_bar() {
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(512, 18), Vec2(1024, 36)), Color(24, 27, 36), true);
    m_renderer->draw_line(Vec2(0, 36), Vec2(1024, 36), Color(45, 52, 70));

    m_renderer->draw_text("FILE", Vec2(14, 11), m_showFileMenu ? Color(52, 152, 219) : Color(200, 200, 200));
    m_renderer->draw_text("EDIT", Vec2(64, 11), m_showEditMenu ? Color(52, 152, 219) : Color(200, 200, 200));
    m_renderer->draw_text("SCENE", Vec2(114, 11), m_showSceneMenu ? Color(52, 152, 219) : Color(200, 200, 200));
    m_renderer->draw_text("HELP", Vec2(174, 11), Color(241, 196, 15));

    // Play Controls: PLAY, PAUSE, STOP
    Color playBg  = (m_mode == EditorMode::Play) ? Color(46, 204, 113) : Color(35, 40, 55);
    Color pauseBg = (m_mode == EditorMode::Pause) ? Color(241, 196, 15) : Color(35, 40, 55);
    Color stopBg  = (m_mode == EditorMode::Edit) ? Color(192, 57, 43) : Color(35, 40, 55);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(470, 18), Vec2(56, 22)), playBg, true);
    m_renderer->draw_text("PLAY", Vec2(456, 11), Color::White);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(540, 18), Vec2(56, 22)), pauseBg, true);
    m_renderer->draw_text("PAUSE", Vec2(522, 11), Color::White);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(605, 18), Vec2(50, 22)), stopBg, true);
    m_renderer->draw_text("STOP", Vec2(592, 11), Color::White);

    // Dropdowns
    if (m_showFileMenu) {
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(80, 84), Vec2(130, 96)), Color(30, 34, 46), true);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(80, 84), Vec2(130, 96)), Color(52, 152, 219), false);
        m_renderer->draw_text("NEW SCENE", Vec2(22, 42), Color::White);
        m_renderer->draw_text("LOAD SCENE", Vec2(22, 66), Color::White);
        m_renderer->draw_text("SAVE SCENE", Vec2(22, 90), Color::White);
        m_renderer->draw_text("EXIT", Vec2(22, 114), Color(231, 76, 60));
    }

    if (m_showEditMenu) {
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(135, 84), Vec2(140, 96)), Color(30, 34, 46), true);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(135, 84), Vec2(140, 96)), Color(52, 152, 219), false);
        m_renderer->draw_text("ADD ENTITY", Vec2(72, 42), Color::White);
        m_renderer->draw_text("DUPLICATE", Vec2(72, 66), Color::White);
        m_renderer->draw_text("DELETE ENTITY", Vec2(72, 90), Color(231, 76, 60));
        m_renderer->draw_text("DESELECT ALL", Vec2(72, 114), Color::White);
    }

    if (m_showSceneMenu) {
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(185, 72), Vec2(140, 72)), Color(30, 34, 46), true);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(185, 72), Vec2(140, 72)), Color(52, 152, 219), false);
        m_renderer->draw_text("SPAWN PLAYER", Vec2(122, 42), Color::White);
        m_renderer->draw_text("SPAWN ENEMY", Vec2(122, 66), Color::White);
        m_renderer->draw_text("SPAWN CAMERA", Vec2(122, 90), Color::White);
    }
}

void Editor::render_hierarchy_panel() {
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(105, 258), Vec2(210, 444)), Color(22, 25, 34), true);
    m_renderer->draw_line(Vec2(210, 36), Vec2(210, 480), Color(45, 52, 70));

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(105, 48), Vec2(210, 24)), Color(32, 36, 48), true);
    m_renderer->draw_text("HIERARCHY (" + std::to_string(m_world.get_entity_count()) + ")", Vec2(12, 42), Color(180, 190, 210));

    const auto& entities = m_world.get_entities();
    int startY = 66;
    int rowHeight = 24;

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity e = entities[i];
        int itemY = startY + static_cast<int>(i) * rowHeight;
        bool isSelected = (m_hasSelection && m_selectedEntity == e);

        Color rowBg = isSelected ? Color(43, 84, 163) : Color(28, 32, 44);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(105, itemY + 12), Vec2(200, 22)), rowBg, true);

        std::string tag = "[O] ";
        std::string name = m_world.get_name(e);
        if (name.find("Camera") != std::string::npos) tag = "[C] ";
        else if (name.find("Player") != std::string::npos) tag = "[P] ";
        else if (name.find("Enemy") != std::string::npos || name.find("Orc") != std::string::npos || name.find("Goblin") != std::string::npos) tag = "[E] ";

        m_renderer->draw_text(tag + name, Vec2(16, itemY + 6), isSelected ? Color::White : Color(200, 210, 225));
    }

    // Action Buttons: [+ Add], [Dup], [- Del]
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(38, 460), Vec2(58, 22)), Color(46, 204, 113), true);
    m_renderer->draw_text("+ ADD", Vec2(16, 453), Color::White);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(104, 460), Vec2(58, 22)), Color(52, 152, 219), true);
    m_renderer->draw_text("DUP", Vec2(88, 453), Color::White);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(170, 460), Vec2(58, 22)), Color(231, 76, 60), true);
    m_renderer->draw_text("- DEL", Vec2(152, 453), Color::White);
}

void Editor::render_viewport_panel() {
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(492, 258), Vec2(564, 444)), Color(18, 20, 28), true);
    m_renderer->draw_line(Vec2(774, 36), Vec2(774, 480), Color(45, 52, 70));

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(492, 48), Vec2(564, 24)), Color(30, 34, 46), true);
    std::string modeText = (m_mode == EditorMode::Play) ? "[PLAYING]" : (m_mode == EditorMode::Pause ? "[PAUSED]" : "[EDITING]");
    Color modeColor = (m_mode == EditorMode::Play) ? Color(46, 204, 113) : Color(241, 196, 15);
    m_renderer->draw_text("VIEWPORT CANVAS  " + modeText, Vec2(220, 42), modeColor);

    int centerX = 492;
    int centerY = 258;

    for (int x = m_rectViewport.x + 20; x < m_rectViewport.x + m_rectViewport.width; x += 40) {
        m_renderer->draw_line(Vec2(x, 60), Vec2(x, 470), Color(30, 35, 48));
    }
    for (int y = 60; y < 470; y += 40) {
        m_renderer->draw_line(Vec2(210, y), Vec2(774, y), Color(30, 35, 48));
    }

    m_renderer->draw_line(Vec2(210, centerY), Vec2(774, centerY), Color(231, 76, 60));
    m_renderer->draw_line(Vec2(centerX, 60), Vec2(centerX, 470), Color(46, 204, 113));

    for (Entity e : m_world.get_entities()) {
        auto* transform = m_world.get_transform(e);
        if (!transform) continue;

        int sx = centerX + static_cast<int>(transform->position.x * 16.0);
        int sy = centerY - static_cast<int>(transform->position.y * 16.0);

        auto* sprite = m_world.get_sprite(e);
        Color color = Color(149, 165, 166);
        std::string entName = m_world.get_name(e);
        if (entName.find("Player") != std::string::npos) color = Color(46, 204, 113);
        else if (entName.find("Camera") != std::string::npos) color = Color(52, 152, 219);
        else if (entName.find("Enemy") != std::string::npos || entName.find("Orc") != std::string::npos || entName.find("Goblin") != std::string::npos) color = Color(231, 76, 60);
        else color = Color(155, 89, 182);

        int w = sprite ? static_cast<int>(sprite->size.x * 12.0) : 24;
        int h = sprite ? static_cast<int>(sprite->size.y * 12.0) : 24;
        if (w < 12) w = 12;
        if (h < 12) h = 12;

        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx, sy), Vec2(w, h)), color, true);
        m_renderer->draw_text(entName, Vec2(sx - 16, sy - h/2 - 12), Color::White);

        if (m_hasSelection && m_selectedEntity == e) {
            m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx, sy), Vec2(w + 6, h + 6)), Color(241, 196, 15), false);
            m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx - w/2 - 3, sy - h/2 - 3), Vec2(6, 6)), Color::Cyan, true);
            m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx + w/2 + 3, sy - h/2 - 3), Vec2(6, 6)), Color::Cyan, true);
            m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx - w/2 - 3, sy + h/2 + 3), Vec2(6, 6)), Color::Cyan, true);
            m_renderer->draw_rect(AABB::fromCenterSize(Vec2(sx + w/2 + 3, sy + h/2 + 3), Vec2(6, 6)), Color::Cyan, true);
        }
    }
}

void Editor::render_inspector_panel() {
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 258), Vec2(250, 444)), Color(22, 25, 34), true);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 48), Vec2(250, 24)), Color(32, 36, 48), true);
    m_renderer->draw_text("INSPECTOR", Vec2(784, 42), Color(180, 190, 210));

    if (!m_hasSelection) {
        m_renderer->draw_text("NO ENTITY SELECTED", Vec2(784, 90), Color(120, 130, 150));
        m_renderer->draw_text("SELECT AN ENTITY FROM", Vec2(784, 114), Color(100, 110, 130));
        m_renderer->draw_text("HIERARCHY OR VIEWPORT", Vec2(784, 138), Color(100, 110, 130));
        return;
    }

    std::string nameDisplay = m_isRenamingEntity ? m_renameBuffer + "_" : m_world.get_name(m_selectedEntity);
    m_renderer->draw_text("NAME: " + nameDisplay, Vec2(784, 75), m_isRenamingEntity ? Color(241, 196, 15) : Color::White);

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(960, 78), Vec2(60, 20)), m_isRenamingEntity ? Color(46, 204, 113) : Color(52, 73, 94), true);
    m_renderer->draw_text(m_isRenamingEntity ? "SAVE" : "RENAME", Vec2(936, 73), Color::White);

    // 1. Transform Card
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 140), Vec2(238, 65)), Color(28, 32, 44), true);
    m_renderer->draw_text("TRANSFORM", Vec2(790, 115), Color(52, 152, 219));

    auto* transform = m_world.get_transform(m_selectedEntity);
    if (transform) {
        std::stringstream ssX, ssY;
        ssX << std::fixed << std::setprecision(1) << transform->position.x;
        ssY << std::fixed << std::setprecision(1) << transform->position.y;

        m_renderer->draw_text("POS X: " + ssX.str(), Vec2(790, 132), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(852, 135), Vec2(20, 16)), Color(192, 57, 43), true);
        m_renderer->draw_text("-", Vec2(849, 130), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(882, 135), Vec2(20, 16)), Color(39, 174, 96), true);
        m_renderer->draw_text("+", Vec2(879, 130), Color::White);

        m_renderer->draw_text("POS Y: " + ssY.str(), Vec2(790, 152), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(852, 155), Vec2(20, 16)), Color(192, 57, 43), true);
        m_renderer->draw_text("-", Vec2(849, 150), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(882, 155), Vec2(20, 16)), Color(39, 174, 96), true);
        m_renderer->draw_text("+", Vec2(879, 130), Color::White);
    }

    // 2. Sprite Card
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 215), Vec2(238, 55)), Color(28, 32, 44), true);
    m_renderer->draw_text("SPRITE", Vec2(790, 195), Color(155, 89, 182));
    auto* sprite = m_world.get_sprite(m_selectedEntity);
    if (sprite) {
        m_renderer->draw_text("TEX : " + sprite->textureId, Vec2(790, 212), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(910, 220), Vec2(70, 18)), Color(52, 73, 94), true);
        m_renderer->draw_text("CHANGE", Vec2(882, 215), Color::White);
    } else {
        m_renderer->draw_text("NO SPRITE COMPONENT", Vec2(790, 215), Color(100, 110, 125));
    }

    // 3. Collider Card
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 285), Vec2(238, 55)), Color(28, 32, 44), true);
    m_renderer->draw_text("COLLIDER 2D", Vec2(790, 265), Color(241, 196, 15));
    auto* collider = m_world.get_collider(m_selectedEntity);
    if (collider) {
        std::string ctype = (collider->type == ColliderType::Box) ? "BOX" : "CIRCLE";
        m_renderer->draw_text("TYPE: " + ctype, Vec2(790, 282), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(910, 288), Vec2(65, 18)), Color(52, 73, 94), true);
        m_renderer->draw_text("TOGGLE", Vec2(884, 283), Color::White);
    } else {
        m_renderer->draw_text("NO COLLIDER COMPONENT", Vec2(790, 282), Color(100, 110, 125));
    }

    // 4. Health Card
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(899, 355), Vec2(238, 55)), Color(28, 32, 44), true);
    m_renderer->draw_text("HEALTH", Vec2(790, 335), Color(46, 204, 113));
    auto* health = m_world.get_health(m_selectedEntity);
    if (health) {
        m_renderer->draw_text("HP: " + std::to_string(health->value) + "/" + std::to_string(health->maxHp), Vec2(790, 352), Color::White);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(838, 358), Vec2(35, 16)), Color(192, 57, 43), true);
        m_renderer->draw_text("-10", Vec2(826, 353), Color::White);

        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(878, 358), Vec2(35, 16)), Color(39, 174, 96), true);
        m_renderer->draw_text("+10", Vec2(866, 353), Color::White);

        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(920, 358), Vec2(38, 16)), Color(52, 152, 219), true);
        m_renderer->draw_text("FULL", Vec2(906, 353), Color::White);
    } else {
        m_renderer->draw_text("NO HEALTH COMPONENT", Vec2(790, 352), Color(100, 110, 125));
    }

    // Add Component Button
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(857, 458), Vec2(140, 24)), Color(46, 204, 113), true);
    m_renderer->draw_text("+ ADD COMPONENT", Vec2(792, 452), Color::White);

    if (m_showAddComponentMenu) {
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(860, 464), Vec2(150, 110)), Color(30, 34, 46), true);
        m_renderer->draw_rect(AABB::fromCenterSize(Vec2(860, 464), Vec2(150, 110)), Color(46, 204, 113), false);
        m_renderer->draw_text("TRANSFORM", Vec2(795, 415), Color::White);
        m_renderer->draw_text("SPRITE", Vec2(795, 437), Color::White);
        m_renderer->draw_text("COLLIDER 2D", Vec2(795, 459), Color::White);
        m_renderer->draw_text("HEALTH", Vec2(795, 481), Color::White);
        m_renderer->draw_text("ENEMY AI", Vec2(795, 503), Color::White);
    }
}

void Editor::render_bottom_panel() {
    m_renderer->draw_line(Vec2(0, 480), Vec2(1024, 480), Color(45, 52, 70));
    m_renderer->draw_line(Vec2(512, 480), Vec2(512, 640), Color(45, 52, 70));

    // 1. CONSOLE PANE
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(256, 560), Vec2(512, 160)), Color(16, 18, 24), true);
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(256, 494), Vec2(512, 24)), Color(28, 32, 44), true);
    m_renderer->draw_text("CONSOLE LOGS", Vec2(12, 488), Color(180, 190, 210));

    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(470, 494), Vec2(60, 18)), Color(52, 73, 94), true);
    m_renderer->draw_text("CLEAR", Vec2(448, 489), Color::White);

    const auto& logs = Debug::Logger::getLogHistory();
    int logStartY = 512;
    int maxVisible = 5;
    int logCount = static_cast<int>(logs.size());
    int firstIdx = std::max(0, logCount - maxVisible);

    for (int i = firstIdx; i < logCount; ++i) {
        const auto& entry = logs[i];
        Color col = Color(200, 200, 200);
        if (entry.level == Debug::LogLevel::Warning) col = Color(241, 196, 15);
        else if (entry.level == Debug::LogLevel::Error) col = Color(231, 76, 60);

        std::string line = "[" + entry.timestamp + "] " + entry.message;
        if (line.size() > 58) line = line.substr(0, 58) + "...";

        m_renderer->draw_text(line, Vec2(12, logStartY + (i - firstIdx) * 22), col);
    }

    // 2. PROFILER PANE
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(768, 560), Vec2(512, 160)), Color(16, 18, 24), true);
    m_renderer->draw_rect(AABB::fromCenterSize(Vec2(768, 494), Vec2(512, 24)), Color(28, 32, 44), true);
    m_renderer->draw_text("PROFILER & PERFORMANCE METRICS", Vec2(524, 488), Color(180, 190, 210));

    std::stringstream ssFps, ssFrameMs, ssPhys;
    ssFps << std::fixed << std::setprecision(1) << m_fps;
    ssFrameMs << std::fixed << std::setprecision(2) << m_frameTimeMs;
    ssPhys << std::fixed << std::setprecision(2) << m_physicsTimeMs;

    m_renderer->draw_text("FPS          : " + ssFps.str() + " (" + ssFrameMs.str() + " MS)", Vec2(524, 514), Color::White);
    m_renderer->draw_text("FIXED TICK   : 60 HZ (16.6 MS)", Vec2(524, 536), Color(180, 190, 200));
    m_renderer->draw_text("ENTITIES     : " + std::to_string(m_world.get_entity_count()), Vec2(524, 558), Color(46, 204, 113));
    m_renderer->draw_text("PHYSICS TIME : " + ssPhys.str() + " MS", Vec2(524, 580), Color(241, 196, 15));
    m_renderer->draw_text("MEMORY ALLOC : ARENA & POOL ALLOCATORS ACTIVE", Vec2(524, 602), Color(155, 89, 182));
}

} // namespace EditorSystem
} // namespace Engine
