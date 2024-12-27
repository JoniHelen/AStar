#pragma once

#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include "Windows.hpp"
#include "Area.hpp"

// Hasher for vector2
template<>
struct std::hash<Vector2> {
    std::size_t operator()(const Vector2& x) const noexcept {
        return x.X * 310248241ull ^ x.Y * 696729599ull;
    }
};

// Comparison for fScore heap
template<>
struct std::greater<std::pair<Vector2, double>> {
    constexpr bool operator()(
        const std::pair<Vector2, double>& lhs,
        const std::pair<Vector2, double>& rhs) const {
        return lhs.second > rhs.second;
    }
};

// Equals for searching
auto operator==(const Vector2& lhs, const Vector2& rhs) -> bool;

namespace AStar {
    class Pathfinder final {
    public:
        enum class Status {
            InProgress,
            Success,
            Error
        };

        Pathfinder();
        [[nodiscard]] auto GetArea() noexcept -> Area&;
        Pathfinder(Vector2 dimensions, const Vector2* obstacles, short obstacleCount) noexcept;

        // Updates the search step
        auto Update() noexcept -> Status;

        // Initializes a new search
        auto Initialize(Vector2 start, Vector2 end) noexcept -> void;

        // Draws the completed path
        auto DrawPath() noexcept -> void;

    private:
        // Reconstructs the completed path from the map
        auto ReconstructPath(const Vector2& end) noexcept -> void;

        // Checks if the tile is valid
        auto IsValid(const Vector2& tile) noexcept -> bool;

        // Manhattan distance to the end point
        auto DistanceToEnd(const Vector2& tile) const noexcept -> double;

        Area _area;
        std::priority_queue<
            std::pair<Vector2, double>,
            std::vector<std::pair<Vector2, double>>,
            std::greater<std::pair<Vector2, double>>
        > _openSet;
        std::unordered_set<Vector2> _closedSet;
        std::unordered_map<Vector2, double> _gScore;
        std::unordered_map<Vector2, double> _fScore;
        std::unordered_map<Vector2, bool> _openSetResidency;
        std::unordered_map<Vector2, Vector2> _cameFrom;
        Vector2 _start, _end;
        std::vector<Vector2> _obstacles;
        std::stack<Vector2> _path;

        std::vector<Vector2> _directions {
            { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 }
        };
    };
} // namespace AStar
