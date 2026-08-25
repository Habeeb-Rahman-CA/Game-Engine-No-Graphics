#ifndef WORLD_TILEMAP_HPP
#define WORLD_TILEMAP_HPP

#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include "Renderer/GPU2DRenderer.hpp"

#include <vector>
#include <string>
#include <memory>

namespace Engine {
namespace WorldSystem {

using Vec2 = Math::Vec2;
using Color = RenderSystem::Color;
using AABB = Math::AABB;

enum class TileType {
    Empty = 0,
    Grass = 1,
    Dirt = 2,
    StoneWall = 3,
    Water = 4,
    Lava = 5,
    WoodFloor = 6,
    Door = 7
};

struct Tile {
    TileType type = TileType::Empty;
    bool isSolid = false;
    Color color = Color(40, 40, 40);
    std::string textureName = "";
};

class Tilemap {
public:
    Tilemap(int width = 32, int height = 24, float tileSize = 1.0f);
    ~Tilemap() = default;

    void initializeDefaultGrid();
    void resize(int width, int height);

    Tile getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);

    bool isSolidAtWorldPos(Vec2 worldPos) const;
    bool checkCollision(const AABB& bounds) const;

    void render(RenderSystem::GPU2DRenderer& renderer, Vec2 cameraOffset);

    bool saveToFile(const std::string& filepath) const;
    bool loadFromFile(const std::string& filepath);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    float getTileSize() const { return m_tileSize; }

    static Tile getTileProperties(TileType type);

private:
    int m_width;
    int m_height;
    float m_tileSize;
    std::vector<TileType> m_tiles; // 1D array representing m_width x m_height
};

} // namespace WorldSystem
} // namespace Engine

#endif // WORLD_TILEMAP_HPP
