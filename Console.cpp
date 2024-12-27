#include "Console.hpp"
#include "Windows.hpp"
#include <format>

namespace AStar {
    auto Console::Clear() noexcept -> void {
        const HANDLE outputHandle = ScreenBuffer[RenderingScreenBufferIndex];
        constexpr COORD cursorOrigin = { 0, 0 }; // Default cursor position
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

        if (!GetConsoleScreenBufferInfo(outputHandle, &screenBufferInfo)) {
            return;
        }

        const DWORD consoleBufferSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;

        // Fill the console buffer with blank characters
        DWORD charsWritten;
        if (!FillConsoleOutputCharacter(outputHandle, ' ', consoleBufferSize, cursorOrigin, &charsWritten)) {
            return;
        }

        if (!GetConsoleScreenBufferInfo(outputHandle, &screenBufferInfo)) {
            return;
        }

        // Write new attributes to the console after updating
        DWORD attrsWritten;
        if (!FillConsoleOutputAttribute(outputHandle, screenBufferInfo.wAttributes, consoleBufferSize, cursorOrigin, &attrsWritten)) {
            return;
        }

        // Set the cursor position to default
        SetConsoleCursorPosition(outputHandle, cursorOrigin);
    }

    auto Console::CreateBuffers() noexcept -> void {
        // Resize window to fit the grid
        SetWindowPos(GetConsoleWindow(), nullptr, 0, 0, 1280, 720, SWP_NOMOVE | SWP_FRAMECHANGED);

        // Create the second buffer
        ScreenBuffer[0] = GetStdHandle(STD_OUTPUT_HANDLE);
        ScreenBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);

        // Enable virtual terminal
        DWORD consoleMode;
        GetConsoleMode(ScreenBuffer[0], &consoleMode);

        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

        SetConsoleMode(ScreenBuffer[0], consoleMode);
        SetConsoleMode(ScreenBuffer[1], consoleMode);

        // Resize the original buffer to increase write performance
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

        if (!GetConsoleScreenBufferInfo(ScreenBuffer[1], &screenBufferInfo)) {
            return;
        }

        SetConsoleScreenBufferSize(ScreenBuffer[0], screenBufferInfo.dwSize);

        // Reserve text space
        writeBuffer.reserve(screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y);
    }

    auto Console::Write(const std::wstring_view str) noexcept -> void {
        writeBuffer += str;
    }

    auto Console::Write(const std::wstring_view str, const Color foreground, const Color background) noexcept -> void {
        writeBuffer += std::format(L"\x1b[{};{}m", static_cast<int>(foreground), static_cast<int>(background));

        Write(str);

        writeBuffer += std::format(L"\x1b[{}m", 0);
    }

    auto Console::Write(const wchar_t chr) noexcept -> void {
        writeBuffer += chr;
    }

    auto Console::Write(const wchar_t chr, const Color foreground, const Color background) noexcept -> void {
        writeBuffer += std::format(L"\x1b[{};{}m", static_cast<int>(foreground), static_cast<int>(background));

        Write(chr);

        writeBuffer += std::format(L"\x1b[{}m", 0);
    }

    auto Console::WriteLine(const std::wstring_view str) noexcept -> void {
        Write(str);
        Write(L"\r\n");
    }

    auto Console::WriteLine(const std::wstring_view str, const Color foreground, const Color background) noexcept -> void {
        Write(str, foreground, background);
        Write(L"\r\n");
    }

    auto Console::WriteLine(const wchar_t chr) noexcept -> void {
        Write(chr);
        Write(L"\r\n");
    }

    auto Console::WriteLine(const wchar_t chr, const Color foreground, const Color background) noexcept -> void {
        Write(chr, foreground, background);
        Write(L"\r\n");
    }

    auto Console::SwapBuffers() noexcept -> void {
        // Clear the console and write to the output buffer
        Clear();
        WriteConsole(ScreenBuffer[RenderingScreenBufferIndex], writeBuffer.c_str(), writeBuffer.length(), nullptr, nullptr);

        // Swap the buffers
        SetConsoleActiveScreenBuffer(ScreenBuffer[RenderingScreenBufferIndex]);
        RenderingScreenBufferIndex = (RenderingScreenBufferIndex + 1) % 2;

        // Clear the write buffer
        writeBuffer.clear();
    }
} // namespace AStar
