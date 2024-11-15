#include <iostream>
#include "Chip8/Chip8.hpp"

Chip8 myChip;

int main(int, char**){
    //Set up rendering system and register input callbacks
    SetupGraphics();
    SetupInput();

    //Initialize Chip8 system and load game;
    myChip.Init();
    myChip.LoadGame("../glitchGhost.ch8");
    while (1)
    {
        //proceed one cycle
        myChip.EmulateCycle();

        //if emulator wants to draw -> draw
        if(myChip.drawFlag)
        {
            drawGraphics();
        }

        //Store key states
        myChip.SetKeys();
    }
    return 0;
}
