#include "World/Tilemap.hpp"
#include "Debug/Logger.hpp"
#include "Platform/FileSystem.hpp"

#include <fstream>
#include <sstream>

namespace Engine {
namespace WorldSystem {

Tilemap::Tilemap(int width, int height, float tileSize)
    : m_width(width), m_height(height), m_tileSize(tileSize) {
    m_tiles.resize(m_width * m_height, TileType::Grass);
    initializeDefaultGrid();
}

Tile Tilemap::getTileProperties(TileType type) {
    Tile t;
    t.type = type;

    switch (type) {
        case TileType::Empty:
            t.isSolid = false;
            t.color = Color(20, 24, 34);
            t.textureName = "";
            break;
        case TileType::Grass:
            t.isSolid = false;
            t.color = Color(46, 204, 113); // Green
            t.textureName = "grass.png";
            break;
        case TileType::Dirt:
            t.isSolid = false;
            t.color = Color(140, 85, 40); // Brown
            t.textureName = "dirt.png";
            break;
        case TileType::StoneWall:
            t.isSolid = true;
            t.color = Color(120, 130, 140); // Gray Stone
            t.textureName = "stone_wall.png";
            break;
        case TileType::Water:
            t.isSolid = true; // Deep water restricts movement
            t.color = Color(52, 152, 219); // Blue Water
            t.textureName = "water.png";
            break;
        case TileType::Lava:
            t.isSolid = true;
            t.color = Color(231, 76, 60); // Red Lava
            t.textureName = "lava.png";
            break;
        case TileType::WoodFloor:
            t.isSolid = false;
            t.color = Color(211, 140, 70); // Wood
            t.textureName = "wood_floor.png";
            break;
        case TileType::Door:
            t.isSolid = false;
            t.color = Color(241, 196, 15); // Yellow Gold Door
            t.textureName = "door.png";
            break;
    }

    return t;
}

void Tilemap::initializeDefaultGrid() {
    // Generate a default dungeon level layout with surrounding stone walls
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1) {
                setTile(x, y, TileType::StoneWall);
            } else if ((x == 5 || x == 6) && (y >= 4 && y <= 12)) {
                setTile(x, y, TileType::StoneWall);
            } else if (y == 16 && (x >= 8 && x <= 22)) {
                setTile(x, y, TileType::Water);
            } else {
                setTile(x, y, TileType::Grass);
            }
        }
    }
}

void Tilemap::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_tiles.clear();
    m_tiles.resize(m_width * m_height, TileType::Grass);
    initializeDefaultGrid();
}

Tile Tilemap::getTile(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return getTileProperties(TileType::StoneWall); // Out of bounds acts as solid wall
    }
    return getTileProperties(m_tiles[y * m_width + x]);
}

void Tilemap::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_tiles[y * m_width + x] = type;
    }
}

bool Tilemap::isSolidAtWorldPos(Vec2 worldPos) const {
    int tx = static_cast<int>(worldPos.x / m_tileSize);
    int ty = static_cast<int>(worldPos.y / m_tileSize);
    return getTile(tx, ty).isSolid;
}

bool Tilemap::checkCollision(const AABB& bounds) const {
    int minX = static_cast<int>(bounds.minBound.x / m_tileSize);
    int maxX = static_cast<int>(bounds.maxBound.x / m_tileSize);
    int minY = static_cast<int>(bounds.minBound.y / m_tileSize);
    int maxY = static_cast<int>(bounds.maxBound.y / m_tileSize);

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (getTile(x, y).isSolid) {
                return true;
            }
        }
    }
    return false;
}

void Tilemap::render(RenderSystem::GPU2DRenderer& renderer, Vec2 cameraOffset) {
    (void)cameraOffset;
    int centerX = 492; // Viewport center
    int centerY = 258;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Tile tile = getTile(x, y);
            if (tile.type == TileType::Empty) continue;

            double worldX = (x - m_width / 2.0) * m_tileSize;
            double worldY = (m_height / 2.0 - y) * m_tileSize;

            int sx = centerX + static_cast<int>(worldX * 16.0);
            int sy = centerY - static_cast<int>(worldY * 16.0);

            int sizePx = static_cast<int>(m_tileSize * 16.0);
            if (sizePx < 8) sizePx = 8;

            renderer.draw_rect(AABB::fromCenterSize(Vec2(sx, sy), Vec2(sizePx, sizePx)), tile.color, true);
            renderer.draw_rect(AABB::fromCenterSize(Vec2(sx, sy), Vec2(sizePx, sizePx)), Color(30, 35, 45), false);
        }
    }
}

bool Tilemap::saveToFile(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;

    out << m_width << " " << m_height << " " << m_tileSize << "\n";
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            out << static_cast<int>(m_tiles[y * m_width + x]) << " ";
        }
        out << "\n";
    }
    LOG_INFO("Tilemap saved successfully to: " + filepath);
    return true;
}

bool Tilemap::loadFromFile(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) return false;

    in >> m_width >> m_height >> m_tileSize;
    m_tiles.resize(m_width * m_height);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int t;
            in >> t;
            m_tiles[y * m_width + x] = static_cast<TileType>(t);
        }
    }
    LOG_INFO("Tilemap loaded successfully from: " + filepath);
    return true;
}

} // namespace WorldSystem
} // namespace Engine
