#ifndef PHYSICS_SPATIALGRID_HPP
#define PHYSICS_SPATIALGRID_HPP

#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include "Entity/Entity.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <utility>

namespace Engine {
namespace Physics {

using Entity = EntitySystem::Entity;
using AABB = Math::AABB;
using Vec2 = Math::Vec2;

struct SpatialCell {
    std::vector<Entity> entities;
};

struct SpatialBenchmarkResult {
    size_t entityCount = 0;
    double bruteForceTimeMs = 0.0;
    double spatialGridTimeMs = 0.0;
    double speedup = 1.0;
};

struct SpatialBenchmarkSuite {
    std::vector<SpatialBenchmarkResult> results;
    size_t totalCellCount = 0;
};

class SpatialGrid {
public:
    SpatialGrid(double cellSize = 4.0, double worldWidth = 1000.0, double worldHeight = 1000.0);
    ~SpatialGrid() = default;

    void clear();
    void insert(Entity entity, const AABB& bounds);
    std::vector<Entity> query(const AABB& bounds) const;

    size_t getCellCount() const { return m_grid.size(); }
    double getCellSize() const { return m_cellSize; }

    // Benchmark Suite for 1,000 / 10,000 / 50,000 / 100,000 entities
    static SpatialBenchmarkSuite runBenchmarkSuite();

private:
    std::pair<int, int> getCellCoords(const Vec2& point) const;

    double m_cellSize;
    double m_worldWidth;
    double m_worldHeight;

    // Hash map mapping cell coordinate pair (x, y) hash to cell
    struct CellHash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    std::unordered_map<std::pair<int, int>, SpatialCell, CellHash> m_grid;
};

} // namespace Physics
} // namespace Engine

#endif // PHYSICS_SPATIALGRID_HPP
