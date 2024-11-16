#include "Chip8/Chip8.hpp"
#include "raylib.h"

Chip8 myChip;



void SetKeys(u8 * keys)
{
    for(u8 i = 0; i < 16; i++)
    {
        //TODO: Keys handling
    }
}

void DrawChip(u8 * pixels)
{
    for (int i = 0; i < CHIP8_SCREEN_HEIGHT; i++)
    {
        for(int j = 0; j < CHIP8_SCREEN_WIDTH; j++)
        {
            if(pixels[i*j + j] == 1)
                DrawRectangle(j * 10, i * 10, 10, 10, BLACK);
        }
    }
}

int main(int, char**){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(24);

    myChip.Init();
    myChip.LoadGame();
    
    while(!WindowShouldClose())
    {
        myChip.EmulateCycle();

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawChip(myChip.GetPixels());

        EndDrawing();
    }    
    return 0;
}


