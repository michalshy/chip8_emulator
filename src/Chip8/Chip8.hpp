#ifndef __CHIP8_HPP
#define __CHIP8_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include "raylib.h"
#include "../Globals.hpp"

// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM

class Chip8{
    u16 opcode;
    u8 memory[4096];
    u8 V[16];
    u16 I;
    u16 pc;
    u8 gfx[64 * 32];
    u8 delay_timer;
    u8 sound_timer;
    u16 stack[16];
    u16 sp;
    u8 key[16];
    bool drawFlag = false;
    Music vfx;
    bool enablePlay = true;
public:
    explicit Chip8() = default;
    void Init();
    void EmulateCycle();
    void LoadGame();
    bool GetDrawFlag(){ return drawFlag; }
    u8 * GetPixels()
    { 
        drawFlag = false;    
        return gfx; 
    }
    u8 * GetKeys(){ return key; }
    ~Chip8();
};

#endif /* __CHIP8_HPP */