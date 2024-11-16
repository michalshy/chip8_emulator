#include "Chip8/Chip8.hpp"
#include "raylib.h"

Chip8 myChip;

u8 keysLayout[16] = 
{'1','2','3','4',
'q','w','e','r',
'a','s','d','f',
'z','x','c','v'};

void SetKeys(unsigned char * keys)
{
    for(u8 i = 0; i < 16; i++)
    {
        
    }
}

int main(int, char**){
    InitWindow(640,320,"chip8_interpreter");

    myChip.Init();
    myChip.LoadGame();
    
    while(!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("yeey!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }    
    return 0;
}


