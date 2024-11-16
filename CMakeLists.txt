cmake_minimum_required(VERSION 3.5.0)
project(CHIP8 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CHIP-8 
    src/main.cpp
    src/Chip8/Chip8.hpp
    src/Chip8/Chip8.cpp
    src/Timer/Timer.hpp)

