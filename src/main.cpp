#include <iostream>
#include <windows.h>
#include "Chip8/Chip8.hpp"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos = {0, 8};

constexpr unsigned short WIDTH = 64;
constexpr unsigned short HEIGHT = 32;

Chip8 myChip;

unsigned char keysLayout[16] = 
{'1','2','3','4',
'q','w','e','r',
'a','s','d','f',
'z','x','c','v'};


void DrawGraphics(unsigned char * beg)
{
    SetConsoleCursorPosition(hConsole, pos);
    for(int i = 0; i < HEIGHT; i ++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            if (beg[i * j + j] == 1)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << " ";
            }
        }
    }
    
}

void SetKeys(unsigned char * keys)
{
    for(int i = 0; i < 16; i++)
    {
        if(GetAsyncKeyState(keysLayout[i]) & 0x8000)
        {
            keys[i] = 1;
        }
    }
}

int main(int, char**){
    //Set up rendering system and register input callbacks
    //SetupGraphics();
    //SetupInput();

    //Initialize Chip8 system and load game;
    myChip.Init();
    myChip.LoadGame();
    while (1)
    {
        //proceed one cycle
        myChip.EmulateCycle();

        //if emulator wants to draw -> draw
        if(myChip.GetDrawFlag())
        {
            DrawGraphics(myChip.GetPixels());
        }

        SetKeys(myChip.GetKeys());
    }
    return 0;
}

