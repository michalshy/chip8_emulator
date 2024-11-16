#ifndef __CHIP8_HPP
#define __CHIP8_HPP

#include <iostream>
#include <windows.h>
#include <chrono>
#include <ctime>

// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM

class Chip8{
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];
    bool drawFlag = false;
public:
    explicit Chip8() = default;
    void Init();
    void EmulateCycle();
    void LoadGame();
    bool GetDrawFlag(){ return drawFlag; }
    unsigned char * GetPixels()
    { 
        drawFlag = false;    
        return gfx; 
    }
    unsigned char * GetKeys(){ return key; }
};

#endif /* __CHIP8_HPP */