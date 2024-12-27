#include "Pathfinder.hpp"
#include <cmath>

auto operator==(const Vector2& lhs, const Vector2& rhs) -> bool {
    return lhs.X == rhs.X && lhs.Y == rhs.Y;
}

namespace AStar {
    Pathfinder::Pathfinder() : _area({ 0, 0 }), _start({ 0, 0 }), _end({ 0, 0 }) {

    }

    auto Pathfinder::GetArea() noexcept -> Area& {
        return _area;
    }

    Pathfinder::Pathfinder(const Vector2 dimensions, const Vector2 *obstacles, const short obstacleCount) noexcept
    : _area(dimensions, L' '), _start(), _end(), _obstacles(obstacles, obstacles + obstacleCount) {
        // Fill the area obstacles
        constexpr Tile obstacle = { L'x', Console::Color::ForegroundBrightRed };

        for (short i = 0; i < obstacleCount; ++i) {
            _area.Set(obstacles[i], obstacle);
        }
    }

    auto Pathfinder::Update() noexcept -> Status {
        if (_openSet.empty()) {
            return Status::Error;
        }

        // Pop the lowest cost tile

        Vector2 current = _openSet.top().first;
        _openSet.pop();
        _openSetResidency.at(current) = false;

        if (current == _end) {
            ReconstructPath(current);
            return Status::Success;
        }

        // Mark the tile as visited

        _closedSet.emplace(current);
        _area.Set(current, {L'0', Console::Color::ForegroundBrightMagenta });

        // Check neighbouring tiles

        for (const auto&[dirX, dirY] : _directions) {
            Vector2 neighbour = { static_cast<short>(current.X + dirX), static_cast<short>(current.Y + dirY) };

            if (!IsValid(neighbour)) {
                continue;
            }

            // Calculate scores and update lists

            double tentative = _gScore.at(current) + 1;

            if (tentative < _gScore.at(neighbour)) {
                _gScore.at(neighbour) = tentative;
                _fScore.at(neighbour) = tentative + DistanceToEnd(neighbour);
                _cameFrom.at(neighbour) = current;

                if (!_openSetResidency.at(neighbour)) {
                    _openSet.emplace(std::make_pair(neighbour, _fScore.at(neighbour)));
                    _area.Set(neighbour, {L'o', Console::Color::ForegroundBrightCyan });
                    _openSetResidency.at(neighbour) = true;
                }
            }
        }

        // Render the updated area

        _area.Render();

        return Status::InProgress;
    }

    auto Pathfinder::Initialize(const Vector2 start, const Vector2 end) noexcept -> void {
        // Clear everything and re-initialize the area
        _area.Clear();

        constexpr Tile obstacle = { L'x', Console::Color::ForegroundBrightRed };

        for (short i = 0; i < _obstacles.size(); ++i) {
            _area.Set(_obstacles[i], obstacle);
        }

        _area.Set(start, { L'S', Console::Color::ForegroundBrightGreen });
        _area.Set(end, { L'E', Console::Color::ForegroundBrightYellow } );

        _start = start;
        _end = end;

        // Sometimes the program just crashes here while releasing the memory from the maps.
        // It's not immediately apparent why that happens, so I can't be bothered to fix it.
        // It could be some form of bad combination of manipulations to the maps

        _gScore = {};
        _fScore = {};
        _cameFrom = {};
        _openSetResidency = {};

        // Fill maps with initial data

        for (short x = 0; x < _area.Width(); ++x) {
            for (short y = 0; y < _area.Height(); ++y) {
                _gScore.emplace(Vector2{ x, y }, std::numeric_limits<double>::infinity());
                _fScore.emplace(Vector2{ x, y }, std::numeric_limits<double>::infinity());
                _cameFrom.emplace(Vector2{ x, y }, Vector2{ -10, -10 });
                _openSetResidency.emplace(Vector2{ x, y }, false);
            }
        }

        while (!_openSet.empty()) {
            _openSet.pop();
        }

        _gScore.at(start) = 0;
        _fScore.at(start) = DistanceToEnd(start);

        _openSet.emplace(std::make_pair(start, _fScore.at(start)));
        _openSetResidency.emplace(std::make_pair(start, true));
    }

    auto Pathfinder::DrawPath() noexcept -> void {
        _area.DrawPath(_path);
    }

    auto Pathfinder::ReconstructPath(const Vector2& end) noexcept -> void{
        while (!_path.empty()) {
            _path.pop();
        }

        _path.emplace(end);

        Vector2 current = end;

        // Traverse the links until the start node is found

        while (_cameFrom.contains(current)) {
            current = _cameFrom.at(current);
            _path.emplace(current);
        }
    }

    auto Pathfinder::IsValid(const Vector2 &tile) noexcept -> bool {
        return tile.X > -1 && tile.Y > -1
            && tile.X < _area.Width() && tile.Y < _area.Height()
            && !std::ranges::contains(_obstacles, tile);
    }

    auto Pathfinder::DistanceToEnd(const Vector2 &tile) const noexcept -> double {
        return std::abs(tile.X - _end.X) + std::abs(tile.Y - _end.Y);
    }
} // namespace AStar
