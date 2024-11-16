cmake_minimum_required(VERSION 3.5.0)
project(CHIP8 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(CHIP-8 
    src/main.cpp
    src/Chip8/Chip8.hpp
    src/Chip8/Chip8.cpp)

