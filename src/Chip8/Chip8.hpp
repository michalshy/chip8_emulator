#ifndef __CHIP8_HPP
#define __CHIP8_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include "raylib.h"
#include "../Globals.hpp"

// Memory classification of CHIP-8
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM

//--------------------CLASS--------------------


/**
 * Class responsible for interpreting CHIP-8 applications
 * Hide all internal functionality, exposes public function as an API for the system
 */

class Chip8{
public:
    /* Public constructor */
    explicit Chip8() = default;
    /* Public setup methods */
    void Init();
    void LoadGame();
    void EmulateCycle();
    void EmulateCycleSecond();
    /* Public getters */
    bool GetDrawFlag(){ return drawFlag; }
    u8 * GetPixels(){ return gfx; }
    u8 * GetKeys(){ return key; }
    /* Public destructor */
    ~Chip8();
private:
    /* Private methods */
    void ExecuteCode();            ///< Executes main opcodes

    /* Aggregators */
    void ZEROS();                  ///< Regulates execution of commands with 0x0 prefix
    void EIGHTS();                 ///< Regulates execution of commands with 0x8 prefix
    void ES();                     ///< Regulates execution of commands with 0xE prefix
    void FS();                     ///< Regulates execution of commands with 0xF prefix
    /* Single commands */
    void CLS();                    ///< 00E0
    void JMP();                    ///< 1nnn
    void RET();                    ///< 00EE
    void CALL();                   ///< 2nnn 
    void SE_BYTE();                ///< 3xkk 
    void SNE_BYTE();               ///< 4xkk 
    void SE_REG();                 ///< 5xy0 
    void LD_BYTE();                ///< 6xkk  
    void ADD_BYTE();               ///< 7xkk  
    void LD_REG();                 ///< 8xy0  
    void OR();                     ///< 8xy1  
    void AND();                    ///< 8xy2  
    void XOR();                    ///< 8xy3  
    void ADD();                    ///< 8xy4  
    void SUB();                    ///< 8xy5  
    void SHR();                    ///< 8xy6  
    void SUBN();                   ///< 8xy7  
    void SHL();                    ///< 8xyE  
    void SNE_REG();                ///< 9xy0  
    void LD_IADDR();               ///< Annn  
    void JMP_VADDR();              ///< Bnnn 
    void RND();                    ///< Cxkk 
    void DRW();                    ///< Dxyn 
    void SKP();                    ///< Ex9E 
    void SKPN();                   ///< ExA1 
    void LD_DT();                  ///< Fx07 
    void LD_K();                   ///< Fx0A 
    void LD_RDT();                 ///< Fx15 
    void LD_RST();                 ///< Fx18 
    void ADD_I();                  ///< Fx1E 
    void LD_RF();                  ///< Fx29 
    void LD_RB();                  ///< Fx33 
    void LD_IREG();                ///< Fx55 
    void LD_RIREG();               ///< Fx65 

    /* Array with functions where index is mapped to mainOpcodes map */
    using chip_func = void (Chip8::*)(); 
    const chip_func instructions[16] = {&Chip8::ZEROS, &Chip8::JMP, &Chip8::CALL, &Chip8::SE_BYTE,
    &Chip8::SNE_BYTE, &Chip8::SE_REG, &Chip8::LD_BYTE, &Chip8::ADD_BYTE, &Chip8::EIGHTS, 
    &Chip8::SNE_REG, &Chip8::LD_IADDR, &Chip8::JMP_VADDR, &Chip8::RND, &Chip8::DRW, 
    &Chip8::ES, &Chip8::FS}; 

    /* Private members */
    u8 memory[4096];                ///< Memory of the system 
    u16 opcode;                     ///< Current opcode
    u8 V[16];                       ///< V register
    u16 I;                          ///< I pointer
    u16 pc;                         ///< Program counter
    u8 gfx[64 * 32];                ///< Matrix of graphical interface
    u8 delay_timer;                 ///< Delay timer
    u8 sound_timer;                 ///< Sound timer
    u16 stack[16];                  ///< 16 bytes stack
    u16 sp;                         ///< Current position on stack
    
    u8 key[16];                     ///< Array provided for key mapping
                                    // Expected keys by system:
                                    // 1    2	3	C
                                    // 4	5	6	D
                                    // 7	8	9	E
                                    // A	0	B	F
    bool drawFlag;                  ///< Flag for optimization of drawing
    Music vfx;                      ///< Music player while sound_timer>0
    bool enablePlay;                ///< Switch for music while sound_timer>0
};



#endif /* __CHIP8_HPP */