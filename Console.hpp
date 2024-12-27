#pragma once

#include <string>
#include "Windows.hpp"

namespace AStar {
    class Console final {
    public:
        // Enum for inputting console colors
        enum Color {
            ForegroundBlack = 30,
            ForegroundBlue = 34,
            ForegroundGreen = 32,
            ForegroundCyan = 36,
            ForegroundRed = 31,
            ForegroundMagenta = 35,
            ForegroundYellow = 33,
            ForegroundWhite = 37,
            ForegroundBrightBlack = 90,
            ForegroundBrightBlue = 94,
            ForegroundBrightGreen = 92,
            ForegroundBrightCyan = 96,
            ForegroundBrightRed = 91,
            ForegroundBrightMagenta = 95,
            ForegroundBrightYellow = 93,
            ForegroundBrightWhite = 97,
            BackgroundBlack = 40,
            BackgroundBlue = 44,
            BackgroundGreen = 42,
            BackgroundCyan = 46,
            BackgroundRed = 41,
            BackgroundMagenta = 45,
            BackgroundYellow = 43,
            BackgroundWhite = 47,
            BackgroundBrightBlack = 100,
            BackgroundBrightBlue = 104,
            BackgroundBrightGreen = 102,
            BackgroundBrightCyan = 106,
            BackgroundBrightRed = 101,
            BackgroundBrightMagenta = 105,
            BackgroundBrightYellow = 103,
            BackgroundBrightWhite = 107,
        };

        // Clears the console output buffer
        static auto Clear() noexcept -> void;

        // Creates console screen buffers
        static auto CreateBuffers() noexcept -> void;

        // Writes to the console write buffer
        static auto Write(std::wstring_view str) noexcept -> void;

        // Writes to the console write buffer with color information
        static auto Write(std::wstring_view str, Color foreground, Color background) noexcept -> void;

        // Writes to the console write buffer
        static auto Write(wchar_t chr) noexcept -> void;

        // Writes to the console write buffer with color information
        static auto Write(wchar_t chr, Color foreground, Color background) noexcept -> void;

        // Writes a line to the console write buffer
        static auto WriteLine(std::wstring_view str) noexcept -> void;

        // Writes a line to the console write buffer with color information
        static auto WriteLine(std::wstring_view str, Color foreground, Color background) noexcept -> void;

        // Writes a line to the console write buffer
        static auto WriteLine(wchar_t chr) noexcept -> void;

        // Writes a line to the console write buffer with color information
        static auto WriteLine(wchar_t chr, Color foreground, Color background) noexcept -> void;

        // Swaps the console screen buffers
        static auto SwapBuffers() noexcept -> void;

    private:
        inline static void* ScreenBuffer[2];
        inline static unsigned char RenderingScreenBufferIndex = 1;
        inline static std::wstring writeBuffer;
    };
} // namespace AStar
