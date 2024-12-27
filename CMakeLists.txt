cmake_minimum_required(VERSION 3.27)
project(AStar)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(AStar main.cpp
        Console.hpp
        Console.cpp
        Windows.hpp
        Pathfinder.cpp
        Pathfinder.hpp
        Area.cpp
        Area.hpp)