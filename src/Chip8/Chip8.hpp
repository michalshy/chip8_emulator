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

/**
 * 
 */

class Chip8{
public:
    //Public Constructor
    explicit Chip8() = default;
    //Public setup methods
    void Init();
    void LoadGame();
    void EmulateCycle();
    void EmulateCycleSecond();
    //Public getters
    bool GetDrawFlag(){ return drawFlag; }
    u8 * GetPixels(){ return gfx; }
    u8 * GetKeys(){ return key; }
    //Public destructor
    ~Chip8();
private:
    //Private methods
    void CLS();
    void RET();
    void JMP();
    void CALL();
    void SE_BYTE();
    void SNE_BYTE();
    void SE_REG();
    void SNE_REG();
    void LD_BYTE();
    void ADD_BYTE();
    void LD_REG();
    void OR();
    void AND();
    void XOR();
    void ADD();
    void SUB();
    void SHR();
    void SUBN();
    void SHL();
    void SNE_REG();
    void LD_IADDR();
    void JMP_VADDR();
    void RND();
    void DRW();
    void SKP();
    void SKPN();
    void LD_DT();
    void LD_K();
    void LD_RDT();
    void LD_RST();
    void ADD_I();
    void LD_RF();
    void LD_RB();
    void LD_IREG();
    void LD_RIREG();
    //Private members
    u8 memory[4096];
    u16 opcode;
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
};

#endif /* __CHIP8_HPP */