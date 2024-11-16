#include "Chip8/Chip8.hpp"

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
            if(pixels[(i*CHIP8_SCREEN_WIDTH) + j] == 0)
            {
                DrawRectangle(j * 10, i * 10, 10, 10, WHITE);
            }
            else if(pixels[(i*CHIP8_SCREEN_WIDTH) + j] == 1)
            {
                DrawRectangle(j * 10, i * 10, 10, 10, BLACK);
            }
        }
    }
}

int main(int, char**){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(6);

    InitAudioDevice();

    myChip.Init();
    myChip.LoadGame();
    
    //First clear in control of raylib
    ClearBackground(WHITE);
    
    while(!WindowShouldClose())
    {
        myChip.EmulateCycle();
        //rest drawing left for handle of CHIP-8
        BeginDrawing();
            if(myChip.GetDrawFlag())
            {
                ClearBackground(WHITE);
                DrawChip(myChip.GetPixels());
            }            
        EndDrawing();
    }    
    return 0;
}


