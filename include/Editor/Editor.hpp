#ifndef EDITOR_EDITOR_HPP
#define EDITOR_EDITOR_HPP

#include "World/World.hpp"
#include "Platform/Window.hpp"
#include "Renderer/GPU2DRenderer.hpp"
#include "Renderer/Camera2D.hpp"
#include "Event/EventBus.hpp"
#include "Scene/SceneSerializer.hpp"
#include "Debug/DebugRenderer.hpp"
#include "Memory/MemoryBenchmark.hpp"
#include "Physics/SpatialGrid.hpp"
#include "UI/HUDSystem.hpp"

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
    void render_asset_browser();
    void render_benchmark_panel();

    // Interaction Helpers
    void handle_mouse_click(int mx, int my);
    void handle_viewport_drag(int mx, int my);
    void select_entity(Engine::WorldSystem::Entity entity);
    void create_default_scene();
    void duplicate_selected_entity();
    void run_memory_benchmark();

    // Engine Core & Systems
    Platform::Window m_window;
    std::unique_ptr<RenderSystem::GPU2DRenderer> m_renderer;
    WorldSystem::World m_world;
    EventSystem::EventBus m_eventBus;
    RenderSystem::Camera2D m_viewportCamera;
    UISystem::HUDSystem* m_hudSystem = nullptr;

    // Editor State
    EditorMode m_mode = EditorMode::Edit;
    WorldSystem::Entity m_selectedEntity = 0;
    bool m_hasSelection = false;
    bool m_isDraggingInViewport = false;
    bool m_showHUDOverlay = true;
    int m_lastDragMx = 0;
    int m_lastDragMy = 0;

    // Debug Visualization Overlays (DebugRenderer Integration)
    bool m_showCollisionBoxes = true;
    bool m_showEntityIds = true;
    bool m_showPhysicsVectors = true;

    // Bottom Panel Tab (0: Console & Profiler, 1: Asset Browser, 2: Benchmark Tool)
    int m_bottomTab = 0;
    std::string m_selectedAsset = "player_hero.png";
    std::vector<std::string> m_assetFiles = {
        "player_hero.png",
        "enemy_orc.png",
        "enemy_goblin.png",
        "box.png",
        "jump.wav",
        "level1.json",
        "game.cfg"
    };

    // Benchmark Results (MemoryBenchmark & SpatialGrid Integration)
    bool m_hasBenchmarkResults = false;
    Memory::BenchmarkResults m_benchmarkResults;

    bool m_hasSpatialBenchmark = false;
    Physics::SpatialBenchmarkSuite m_spatialSuite;

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

    // Subsystem Profiler Timings
    double m_frameTimeMs = 16.6;
    double m_fps = 60.0;
    double m_physicsTimeMs = 1.2;
    double m_ecsTimeMs = 0.7;
    double m_aiTimeMs = 0.4;
    double m_renderTimeMs = 3.1;
    int m_frameCount = 0;
};

} // namespace EditorSystem
} // namespace Engine

#endif // EDITOR_EDITOR_HPP
