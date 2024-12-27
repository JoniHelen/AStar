#include "Console.hpp"
#include "Pathfinder.hpp"
#include "Windows.hpp"
#include <random>

using namespace AStar;

int main() {
    // Create screen buffers for drawing
    Console::CreateBuffers();

    // Obstacles for the algorithm to avoid
    Vector2 obstacles[] = {
        { 11, 5 }, { 12, 5 }, { 13, 5 }, { 14, 5 }, { 15, 5 }, { 15, 6 }, { 15, 7 }, { 15, 8 }, { 15, 9 },
        { 2, 15 }, { 3, 15 }, { 4, 15 }, { 5, 15 }, { 6, 15 }, { 7, 15 }, { 8, 15 }, { 9, 15 }, { 10, 15 },
        { 0, 9 }, { 1, 9 }, { 2, 9 }, { 3, 9 }, { 4, 9 }, { 5, 9 }, { 6, 9 }, { 7, 9 }, { 8, 9 },
        { 13, 12 }, { 14, 12 }, { 15, 12 }, { 16, 12 }, { 17, 12 }, { 13, 13 }, { 13, 14 }, { 13, 15 }, { 13, 16 }
    };

    // Creating the pathfinder
    Pathfinder pathfinder({ 20, 20 }, obstacles, std::size(obstacles));

    // Random distributions for the end points
    std::random_device device;
    std::uniform_int_distribution<short> distX(1, 18);
    std::uniform_int_distribution<short> distY(1, 3);

    // Loop with different end points
    while (true) {
        pathfinder.Initialize({ 1, 18 }, { distX(device), distY(device) });

        bool findingPath = true;

        // Update cycle

        while (findingPath) {
            Console::Clear();
            switch (pathfinder.Update()) {
            case Pathfinder::Status::InProgress:
                break;

            case Pathfinder::Status::Success:
                findingPath = false;
                break;

            case Pathfinder::Status::Error:
                findingPath = false;
                break;

            default:
                break;
            }
            Console::SwapBuffers();
        }

        // After finding the path, draw it and display for a few seconds.

        pathfinder.DrawPath();
        Console::SwapBuffers();

        Sleep(2000);
    }
}
