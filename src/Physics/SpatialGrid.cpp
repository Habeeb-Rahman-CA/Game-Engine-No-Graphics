#include "Physics/SpatialGrid.hpp"
#include "Debug/Logger.hpp"
#include <chrono>
#include <random>
#include <algorithm>
#include <cmath>

namespace Engine {
namespace Physics {

SpatialGrid::SpatialGrid(double cellSize, double worldWidth, double worldHeight)
    : m_cellSize(cellSize), m_worldWidth(worldWidth), m_worldHeight(worldHeight) {
}

std::pair<int, int> SpatialGrid::getCellCoords(const Vec2& point) const {
    int cx = static_cast<int>(std::floor(point.x / m_cellSize));
    int cy = static_cast<int>(std::floor(point.y / m_cellSize));
    return {cx, cy};
}

void SpatialGrid::clear() {
    m_grid.clear();
}

void SpatialGrid::insert(Entity entity, const AABB& bounds) {
    auto minCell = getCellCoords(bounds.minBound);
    auto maxCell = getCellCoords(bounds.maxBound);

    for (int x = minCell.first; x <= maxCell.first; ++x) {
        for (int y = minCell.second; y <= maxCell.second; ++y) {
            m_grid[{x, y}].entities.push_back(entity);
        }
    }
}

std::vector<Entity> SpatialGrid::query(const AABB& bounds) const {
    std::unordered_set<Entity> resultSet;
    auto minCell = getCellCoords(bounds.minBound);
    auto maxCell = getCellCoords(bounds.maxBound);

    for (int x = minCell.first; x <= maxCell.first; ++x) {
        for (int y = minCell.second; y <= maxCell.second; ++y) {
            auto it = m_grid.find({x, y});
            if (it != m_grid.end()) {
                for (Entity e : it->second.entities) {
                    resultSet.insert(e);
                }
            }
        }
    }

    return std::vector<Entity>(resultSet.begin(), resultSet.end());
}

SpatialBenchmarkSuite SpatialGrid::runBenchmarkSuite() {
    SpatialBenchmarkSuite suite;
    std::vector<size_t> testCounts = {1000, 10000, 50000, 100000};

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> distPos(-500.0, 500.0);
    std::uniform_real_distribution<double> distSize(1.0, 3.0);

    for (size_t N : testCounts) {
        SpatialBenchmarkResult res;
        res.entityCount = N;

        std::vector<AABB> boxes;
        boxes.reserve(N);
        for (size_t i = 0; i < N; ++i) {
            Vec2 center(distPos(rng), distPos(rng));
            Vec2 size(distSize(rng), distSize(rng));
            boxes.push_back(AABB::fromCenterSize(center, size));
        }

        // 1. Brute Force Collision Checks (Sub-sampled if N is very large to prevent multi-minute freeze)
        size_t bruteSamples = (N <= 10000) ? N : 2000;
        auto t0 = std::chrono::high_resolution_clock::now();
        size_t bruteHits = 0;

        for (size_t i = 0; i < bruteSamples; ++i) {
            for (size_t j = i + 1; j < N; ++j) {
                if (boxes[i].intersects(boxes[j])) {
                    bruteHits++;
                }
            }
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double rawBruteTime = std::chrono::duration<double, std::milli>(t1 - t0).count();

        // Extrapolate full brute force time for large N
        if (N > bruteSamples) {
            double ratio = static_cast<double>(N * N) / static_cast<double>(bruteSamples * N);
            res.bruteForceTimeMs = rawBruteTime * ratio;
        } else {
            res.bruteForceTimeMs = rawBruteTime;
        }

        // 2. Spatial Grid Collision Checks
        SpatialGrid grid(8.0, 1000.0, 1000.0);
        auto t2 = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < N; ++i) {
            grid.insert(static_cast<Entity>(i + 1), boxes[i]);
        }

        size_t gridHits = 0;
        for (size_t i = 0; i < N; ++i) {
            auto candidates = grid.query(boxes[i]);
            for (Entity other : candidates) {
                size_t j = static_cast<size_t>(other - 1);
                if (j > i && boxes[i].intersects(boxes[j])) {
                    gridHits++;
                }
            }
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        res.spatialGridTimeMs = std::chrono::duration<double, std::milli>(t3 - t2).count();

        if (res.spatialGridTimeMs > 0.0) {
            res.speedup = res.bruteForceTimeMs / res.spatialGridTimeMs;
        } else {
            res.speedup = 100.0;
        }

        suite.results.push_back(res);
        LOG_INFO("SpatialGrid Benchmark [N=" + std::to_string(N) + "]: BruteForce=" + 
                 std::to_string(res.bruteForceTimeMs) + "ms, SpatialGrid=" + 
                 std::to_string(res.spatialGridTimeMs) + "ms (Speedup: " + 
                 std::to_string(res.speedup) + "x)");
    }

    return suite;
}

} // namespace Physics
} // namespace Engine
