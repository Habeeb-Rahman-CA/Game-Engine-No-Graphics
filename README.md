# Basic 2D Game Engine & Graphical Editor

A high-performance, modular C++17 2D Game Engine and Editor UI built from scratch. Inspired by modern engine workflows (Godot / Unity), it features an Entity-Component-System (ECS) architecture, spatial partitioning for O(N) collision scalability, a 2D tilemap level system, an anchored HUD framework, and a clean public SDK (`EngineSDK.hpp`).

---

## Key Subsystems & Features

### 1. Graphical Editor (1024x640)
- **Top Menu Bar**: Interactivity for `File`, `Edit`, `Scene`, `Guide`, and Play/Pause/Stop simulation controls.
- **Scene Hierarchy Panel**: Live view of scene entities (`Main_Camera`, `Player_Hero`, `Orc_Warrior`, `Goblin_Scout`) with quick entity creation, duplication, and selection.
- **Inspector Panel**: Real-time property editing for selected entities (`Transform`, `Sprite`, `Collider`, `Health`).
- **Interactive Viewport Panel**: 2D camera viewport with mouse picking, object transformation, and toggleable diagnostic overlays (`[MAP]`, `[HUD]`, `[BOXES]`, `[IDS]`, `[VECTORS]`).
- **Asset Browser & Profiler**: Dedicated bottom dock showing asset file inspection (`Assets/player_hero.png`, `game.cfg`) and frame execution profiler metrics.

---

### 2. Spatial Hash Grid Collision Optimization
- Upgrades brute-force collision handling O(N^2) to uniform spatial hash partitioning O(N).
- Tested and benchmarked to support 100,000+ simultaneous 2D entities with high-frame-rate physics.

---

### 3. Tilemap Level System (`Tilemap.hpp` / `Tilemap.cpp`)
- 2D grid-based map management (`Grass`, `Dirt`, `StoneWall`, `Water`, `Lava`).
- Fast raycast/coordinate solid collision checking (`isSolidAtWorldPos`).
- Scene serialization to JSON-style files for persistent level design.

---

### 4. Anchored HUD System (`HUDSystem.hpp` / `HUDSystem.cpp`)
- Component-driven UI framework with screen-space anchoring (`TopLeft`, `TopCenter`, `BottomCenter`).
- Integrated Viewport-constrained HUD overlay featuring HP progress bar, score counter, and hotbar guide.

---

### 5. Public Engine SDK (`EngineSDK.hpp`)
- Provides a clean API boundary separating engine internals from game application logic.
- Enables game developers to create standalone applications using `Engine::SDK::GameEngine`.

---

### 6. Showcase Demo: Snake Eater 2D (`SnakeGame/`)
- A complete, playable 2D Snake game built entirely on top of `EngineSDK.hpp`.
- Features smooth grid movement, food apple spawning, body growth, high score tracking, WASD/Arrow key controls, and on-screen HUD text overlays.

---

## Build & Installation

### Prerequisites
- Linux OS
- C++17 compatible compiler (`g++` / `clang++`)
- GNU `make`
- X11 development headers (`libx11-dev`)

### Building & Running

#### 1. Launch the Game Engine Editor
```bash
make
./build/game_engine
```

#### 2. Launch the Snake Game Showcase Demo
```bash
make snake_game
./build/snake_game
```

#### 3. Clean Build Artifacts
```bash
make clean
```

---

## Repository Structure

```
Basic Game Engine/
├── include/                  # Engine Headers
│   ├── Core/                 # Engine loop & state machine
│   ├── Debug/                # Logger & profiling tools
│   ├── Editor/               # Godot/Unity-style Editor UI
│   ├── Entity/               # ECS components (Transform, Sprite, Health, etc.)
│   ├── EngineSDK.hpp         # Public SDK single header
│   ├── Physics/              # Collision detection & Spatial Hash Grid
│   ├── Platform/             # Platform window, input & filesystem
│   ├── Renderer/             # GPU2DRenderer & canvas backends
│   ├── UI/                   # Anchored HUD system
│   └── World/                # ECS World manager & Tilemap system
├── src/                      # Engine Source Code
│   ├── Core/                 # Core loop & input backends
│   ├── Debug/                # Logger & debug renderers
│   ├── Editor/               # Editor panel layout & click handling
│   ├── Physics/              # Physics system & spatial hash grid implementation
│   ├── Platform/             # X11 Windowing & platform layer
│   ├── Renderer/             # Renderer pipeline implementations
│   ├── UI/                   # HUDSystem implementation
│   └── World/                # World & Tilemap implementation
├── SnakeGame/                # Showcase Demo Game
│   ├── config/game.cfg       # Game configuration
│   └── src/main.cpp          # Snake game logic using EngineSDK.hpp
├── build/                    # Compiled Executables
├── Makefile                  # Build system script
└── README.md                 # Project Documentation
```

---

## Controls

### Game Engine Editor
- **Left Mouse Click**: Select entities in Viewport or Hierarchy panel, trigger top menu items (`FILE`, `EDIT`, `SCENE`, `GUIDE`), and toggle Viewport overlay buttons (`[MAP]`, `[HUD]`, `[BOXES]`, `[IDS]`, `[VECTORS]`).
- **PLAY / PAUSE / STOP**: Control live scene simulation tick.

### Snake Game Demo
- **`W` / `A` / `S` / `D` or `Arrow Keys`**: Change snake direction.
- **`SPACE` or `R`**: Retry / Reset game after Game Over.

---

## License
This engine is built as an open-source educational game engine foundation. Feel free to extend and modify!
