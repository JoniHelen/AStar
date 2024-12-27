#include "Area.hpp"
#include "Console.hpp"

namespace AStar {
    Area::Area() noexcept = default;

    Area::Area(const Vector2 dimensions) noexcept : _width(dimensions.X), _height(dimensions.Y) {
        _data = new Tile[_width * _height];
    }

    Area::Area(const Vector2 dimensions, const wchar_t fill) noexcept : _width(dimensions.X), _height(dimensions.Y) {
        _data = new Tile[_width * _height];
        for (int i = 0, n = _width * _height; i < n; i++) {
            _data[i].character = fill;
            _data[i].color = Console::Color::ForegroundWhite;
        }
    }

    Area::~Area() noexcept {
        delete[] _data;
    }

    auto Area::Width() const noexcept -> short {
        return _width;
    }

    auto Area::Height() const noexcept -> short {
        return _height;
    }

    auto Area::Get(const Vector2 pos) noexcept -> Tile& {
        return _data[pos.Y * _width + pos.X];
    }

    auto Area::Set(const Vector2 pos, const Tile& tile) noexcept -> void {
        _data[pos.Y * _width + pos.X] = tile;
    }

    auto Area::Render() const noexcept -> void {

        // Draw top border
        Console::Write(L"╭");
        for (int x = 0, n = _width - 1; x < n; ++x) {
            Console::Write(L"───┬");
        }
        Console::WriteLine(L"───╮");

        // Draw content
        for (int y = 0; y < _height; ++y) {
            Console::Write(L"│"); // Side wall
            for (int x = 0, n = _width - 1; x < n; ++x) {
                // Get the current tile
                const auto& [character, color] = _data[y * _width + x];

                // Get information about path when rendering it.
                const bool iHaveBlock = _data[y * _width + x].character == L'█';
                const bool lastWasBlock = _data[y * _width + x - 1].character == L'█';
                const bool nextIsBlock = _data[y * _width + x + 1].character == L'█';

                // Draw different things if drawing the path.
                Console::Color drawColor = x > 0 && iHaveBlock && lastWasBlock ? color : Console::Color::ForegroundWhite;
                Console::Write(x > 0 && iHaveBlock && lastWasBlock ? L"█" : L" ", drawColor, Console::Color::BackgroundBlack);
                Console::Write(character, color, Console::Color::BackgroundBlack);
                drawColor = iHaveBlock && nextIsBlock ? color : Console::Color::ForegroundWhite;
                Console::Write(iHaveBlock && nextIsBlock ? L"██" : L" │", drawColor, Console::Color::BackgroundBlack);
            }

            // Draw the right edge of the row.
            const auto& [character, color] = _data[y * _width + _width - 1];

            const bool iHaveBlock = _data[y * _width + _width - 1].character == L'█';
            const bool lastWasBlock = _data[y * _width + _width - 2].character == L'█';

            Console::Color drawColor = iHaveBlock && lastWasBlock ? color : Console::Color::ForegroundWhite;
            Console::Write(iHaveBlock && lastWasBlock ? L"█" : L" ", drawColor, Console::Color::BackgroundBlack);
            Console::Write(character, color, Console::Color::BackgroundBlack);
            Console::WriteLine(L" │");

            // Draw bottom border
            if (y == _height - 1) {
                // This is the last row
                Console::Write(L"╰");
                for (int x = 0, n = _width - 1; x < n; ++x) {
                    Console::Write(L"───┴");
                }
                Console::WriteLine(L"───╯");
            }
            else {
                Console::Write(L"├");
                for (int x = 0, n = _width - 1; x < n; ++x) {

                    // Again information about the path.
                    const bool iHaveBlock = _data[y * _width + x].character == L'█';
                    const bool downIsBlock = _data[(y + 1) * _width + x].character == L'█';

                    if (y < _height - 1 && iHaveBlock && downIsBlock) {
                        Console::Write(L"─");
                        Console::Write(L"█", Console::Color::ForegroundBrightGreen, Console::Color::BackgroundBlack);
                        Console::Write(L"─┼");
                    }
                    else {
                        Console::Write(L"───┼");
                    }
                }

                // Draw the right edge of the row.
                const bool iHaveBlock = _data[y * _width + _width - 1].character == L'█';
                const bool downIsBlock = _data[(y + 1) * _width + _width - 1].character == L'█';

                if (y < _height - 1 && iHaveBlock && downIsBlock) {
                    Console::Write(L"─");
                    Console::Write(L"█", Console::Color::ForegroundBrightGreen, Console::Color::BackgroundBlack);
                    Console::WriteLine(L"─┤");
                }
                else {
                    Console::WriteLine(L"───┤");
                }
            }
        }
    }

    auto Area::DrawPath(std::stack<Vector2>& path) noexcept -> void {
        // Set the tile symbols and then render
        while (!path.empty()) {
            Set(path.top(), { L'█', Console::Color::ForegroundBrightGreen });
            path.pop();
        }

        Render();
    }

    auto Area::Clear() noexcept -> void {
        // Reset all tiles to blank
        for (int i = 0, n = _width * _height; i < n; i++) {
            _data[i].character = L' ';
            _data[i].color = Console::Color::ForegroundWhite;
        }
    }
} // namespace AStar