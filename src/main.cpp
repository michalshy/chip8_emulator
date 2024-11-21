#include "Chip8/Chip8.hpp"
#include "raylib.h"
#include "WindowManager/WindowManager.hpp"
Chip8 chip;
WindowManager wm;

int main(int, char**){
    wm.Loop(chip);

    return 0;
}


