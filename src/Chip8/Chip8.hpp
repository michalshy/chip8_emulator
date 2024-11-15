#ifndef __CHIP8_HPP
#define __CHIP8_HPP

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
public:
    explicit Chip8() = default;
    void Init();
    void EmulateCycle();
    void LoadGame(char* path);
};

#endif /* __CHIP8_HPP */