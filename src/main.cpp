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

void CheckInput(u8 * key)
{
    for(int i = 0; i < 16; i++)
    {
        key[i] = 0;
    }

    if(IsKeyDown(KEY_KP_1)) key[0] = 1;    
    if(IsKeyDown(KEY_KP_2)) key[1] = 1;    
    if(IsKeyDown(KEY_KP_3)) key[2] = 1;    
    if(IsKeyDown(KEY_KP_4)) key[3] = 1;    
    if(IsKeyDown(KEY_Q)) key[4] = 1;    
    if(IsKeyDown(KEY_W)) key[5] = 1;    
    if(IsKeyDown(KEY_E)) key[6] = 1;    
    if(IsKeyDown(KEY_R)) key[7] = 1;    
    if(IsKeyDown(KEY_A)) key[8] = 1;    
    if(IsKeyDown(KEY_S)) key[9] = 1;    
    if(IsKeyDown(KEY_D)) key[10] = 1;    
    if(IsKeyDown(KEY_F)) key[11] = 1;    
    if(IsKeyDown(KEY_Z)) key[12] = 1;    
    if(IsKeyDown(KEY_X)) key[13] = 1;    
    if(IsKeyDown(KEY_C)) key[14] = 1;    
    if(IsKeyDown(KEY_V)) key[15] = 1;    
}

int main(int, char**){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    SetTargetFPS(120);

    InitAudioDevice();

    myChip.Init();
    myChip.LoadGame();
    
    //First clear in control of raylib
    ClearBackground(WHITE);
    
    while(!WindowShouldClose())
    {
        CheckInput(myChip.GetKeys());

        myChip.EmulateCycle();
        //rest drawing left for handle of CHIP-8
        BeginDrawing();
            if(myChip.GetDrawFlag())
            {
                DrawChip(myChip.GetPixels());
            }            
        EndDrawing();

    }    
    return 0;
}


