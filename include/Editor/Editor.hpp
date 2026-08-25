#ifndef EDITOR_EDITOR_HPP
#define EDITOR_EDITOR_HPP

#include "World/World.hpp"
#include "Platform/Window.hpp"
#include "Renderer/GPU2DRenderer.hpp"
#include "Renderer/Camera2D.hpp"
#include "Event/EventBus.hpp"
#include "Scene/SceneSerializer.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Engine {
namespace EditorSystem {

enum class EditorMode {
    Edit,
    Play,
    Pause
};

struct RectUI {
    int x, y, width, height;
    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

class Editor {
public:
    Editor();
    ~Editor() = default;

    bool initialize(int width = 1024, int height = 640);
    void run();

private:
    void process_input();
    void update(double dt);
    void render();

    // UI Panel Rendering Subroutines
    void render_top_bar();
    void render_hierarchy_panel();
    void render_viewport_panel();
    void render_inspector_panel();
    void render_bottom_panel();

    // Interaction Helpers
    void handle_mouse_click(int mx, int my);
    void handle_viewport_drag(int mx, int my);
    void select_entity(Engine::WorldSystem::Entity entity);
    void create_default_scene();
    void duplicate_selected_entity();

    // Engine Core & Systems
    Platform::Window m_window;
    std::unique_ptr<RenderSystem::GPU2DRenderer> m_renderer;
    WorldSystem::World m_world;
    EventSystem::EventBus m_eventBus;
    RenderSystem::Camera2D m_viewportCamera;

    // Editor State
    EditorMode m_mode = EditorMode::Edit;
    WorldSystem::Entity m_selectedEntity = 0;
    bool m_hasSelection = false;
    bool m_isDraggingInViewport = false;
    int m_lastDragMx = 0;
    int m_lastDragMy = 0;

    // Entity Renaming State
    bool m_isRenamingEntity = false;
    std::string m_renameBuffer = "";

    // UI Rect Layout Bounds
    RectUI m_rectTopBar;
    RectUI m_rectHierarchy;
    RectUI m_rectViewport;
    RectUI m_rectInspector;
    RectUI m_rectConsole;
    RectUI m_rectProfiler;

    // Menu States
    bool m_showFileMenu = false;
    bool m_showEditMenu = false;
    bool m_showSceneMenu = false;
    bool m_showAddComponentMenu = false;
    bool m_showHelpModal = false;

    // Console Filter (0: All, 1: Info, 2: Warn, 3: Error)
    int m_consoleFilter = 0;

    // Performance Stats
    double m_frameTimeMs = 16.6;
    double m_fps = 60.0;
    double m_physicsTimeMs = 0.15;
    int m_frameCount = 0;
};

} // namespace EditorSystem
} // namespace Engine

#endif // EDITOR_EDITOR_HPP
