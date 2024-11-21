#include "WindowManager.hpp"

void WindowManager::DrawChip(u8 *pixels)
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

void WindowManager::CheckInput(u8 * key)
{
    for(int i = 0; i < 16; i++)
    {
        key[i] = 0;
    }
    if(IsKeyDown(KEY_ONE)) key[0] = 1;    
    if(IsKeyDown(KEY_TWO)) key[1] = 1;    
    if(IsKeyDown(KEY_THREE)) key[2] = 1;    
    if(IsKeyDown(KEY_FOUR)) key[12] = 1;    
    if(IsKeyDown(KEY_Q)) key[3] = 1;    
    if(IsKeyDown(KEY_W)) key[4] = 1;    
    if(IsKeyDown(KEY_E)) key[5] = 1;    
    if(IsKeyDown(KEY_R)) key[13] = 1;    
    if(IsKeyDown(KEY_A)) key[6] = 1;    
    if(IsKeyDown(KEY_S)) key[7] = 1;    
    if(IsKeyDown(KEY_D)) key[8] = 1;    
    if(IsKeyDown(KEY_F)) key[14] = 1;    
    if(IsKeyDown(KEY_Z)) key[9] = 1;    
    if(IsKeyDown(KEY_X)) key[10] = 1;    
    if(IsKeyDown(KEY_C)) key[11] = 1;    
    if(IsKeyDown(KEY_V)) key[15] = 1;    
}

void WindowManager::Loop(Chip8& chip)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitAudioDevice();
    SetTargetFPS(200);
    
    chip.Init();
    chip.LoadGame();
    
    Music sound = LoadMusicStream(MUSIC_PATH);
    PlayMusicStream(sound);
    
    while(!WindowShouldClose())
    {
        UpdateMusicStream(sound);
        if(chip.GetSoundTimer())
            ResumeMusicStream(sound);
        else
            PauseMusicStream(sound);
            
        chip.EmulateCycle();
        CheckInput(chip.GetKeys());
        BeginDrawing();
            if(chip.GetDrawFlag())
            {
                DrawChip(chip.GetPixels());
            }            
        EndDrawing();
    }    
    CloseAudioDevice();
    CloseWindow();
}
