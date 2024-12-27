#pragma once
#include "Console.hpp"
#include "Windows.hpp"
#include <stack>

namespace AStar {
    // Helper struct for character information
    struct Tile {
        wchar_t character;
        Console::Color color;
    };

    class Area final {
    public:
        Area() noexcept;
        explicit Area(Vector2 dimensions) noexcept;
        Area(Vector2 dimensions, wchar_t fill) noexcept;
        ~Area() noexcept;

        [[nodiscard]] auto Width() const noexcept -> short;
        [[nodiscard]] auto Height() const noexcept -> short;

        // Gets tile information from the area
        [[nodiscard]] auto Get(Vector2 pos) noexcept -> Tile&;

        // Sets the tile information at this location
        auto Set(Vector2 pos, const Tile& tile) noexcept -> void;

        // Renders the area to the buffer
        auto Render() const noexcept -> void;

        // Draws the path
        auto DrawPath(std::stack<Vector2>& path) noexcept -> void;

        // Clears the area
        auto Clear() noexcept -> void;

    private:
        short _width = 0, _height = 0;
        Tile* _data;
    };
} // namespace AStar
