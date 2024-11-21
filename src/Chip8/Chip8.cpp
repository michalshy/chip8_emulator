#include "Chip8.hpp"
#include <cstdio>
#include <time.h>
#include <stdlib.h>

//--------------------PUBLIC METHODS--------------------

void Chip8::Init()
{
    //initialize time
    srand(time(NULL)); 

    //initialize bools
    enablePlay = true;

    //load sound
    vfx = LoadMusicStream("../resources/retro.wav");

    //initialize timers
    sound_timer = 60;
    delay_timer = 60;

    // initialize registers and memory once
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;
    //clear memory
    for(int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
    //load fontset into memory
    for(int i = 0; i < 80; i++)
    {
        memory[i] = CHIP8_FONTSET[i];
    }
    //clear display
    for(int i = 0; i < (CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH); i++)
    {
        gfx[i] = 0;
    }
    //clear keys
    for(int i = 0; i < 16; i++)
    {
        key[i] = 0;
    }

    //clear once
    drawFlag = true;
}

void Chip8::LoadGame()
{
    FILE *file;
    file = fopen("../games/pong1.ch8", "rb");
    if(file == NULL)
    {
        perror("Error opening file!");
        return;
    }

    u8 c;
    u64 i = 0;
    while(fread(&c, sizeof(c), 1, file) == 1)
    {
        memory[i + 512] = c;
        i++;        
    }

    fclose(file);
}

void Chip8::EmulateCycle()
{
    drawFlag = false;

    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    pc += 2;

    (this->*instructions[mappedMainOpcodes[opcode & 0xF000]])();

    // Update timers
    if(delay_timer > 0)
    {
        --delay_timer;
    }
    if(sound_timer > 0)
    {
        //UpdateMusicStream(vfx);
        if(enablePlay)
        {
            //PlayMusicStream(vfx);
            enablePlay = false;
        }
        sound_timer--;
    }
    else if(sound_timer == 0)
    {
        enablePlay = true;
    }
}

Chip8::~Chip8()
{
    UnloadMusicStream(vfx);
}

//--------------------PRIVATE METHODS--------------------

/// @brief Section containing functions for aggregating other functions
/// Check Map section in Globals.hpp
void Chip8::ZEROS()
{
    (this->*instructions[mappedZeroOpcodes[opcode & 0x000F]])();
}
void Chip8::EIGHTS()
{
    (this->*instructions[mappedEightOpcodes[opcode & 0x000F]])();
}
void Chip8::ES()
{
    (this->*instructions[mappedEsOpcodes[opcode & 0x00FF]])();
}
void Chip8::FS()
{
    (this->*instructions[mappedFsOpcodes[opcode & 0x00FF]])();
}
///End of section

/// @brief 00E0 opcode
void Chip8::CLS()
{
    printf("execute %x \n", opcode);
    for(int i = 0; i < (64 * 32); i++)
    {
        gfx[i] = 0;
    }
    drawFlag = true;
}
/// @brief 00EE opcode
void Chip8::RET()
{
    printf("execute %x \n", opcode);
    --sp;
    pc = stack[sp];
}
/// @brief 1nnn opcode
void Chip8::JMP()
{
    printf("execute %x \n", opcode);
    pc = opcode & 0x0FFF;
}
/// @brief 2nnn opcode
void Chip8::CALL()
{
    printf("execute %x \n", opcode);
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
}
/// @brief 3xkk opcode
void Chip8::SE_BYTE()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] == (opcode & 0x00FF))
    {
        pc+=2;
    }
}
/// @brief 4xkk opcode
void Chip8::SNE_BYTE()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] != (opcode & 0x00FF))
    {
        pc+=2;
    }
}
/// @brief 5xy0 opcode
void Chip8::SE_REG()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] == (V[(opcode >> 4) & 0x000F]))
    {
        pc+=2;
    }
}
/// @brief 6xkk opcode
void Chip8::LD_BYTE()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (opcode & 0x00FF);
}
/// @brief 7xkk opcode
void Chip8::ADD_BYTE()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] += (opcode & 0x00FF);
}
/// @brief 8xy0 opcode
void Chip8::LD_REG()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = V[(opcode >> 4) & 0x000F];
}
/// @brief 8xy1 opcode
void Chip8::OR()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] | V[(opcode >> 4) & 0x000F]);
}
/// @brief 8xy2 opcode
void Chip8::AND()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] & V[(opcode >> 4) & 0x000F]);
}
/// @brief 8xy3 opcode
void Chip8::XOR()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] ^ V[(opcode >> 4) & 0x000F]);
}
/// @brief 8xy4 opcode
void Chip8::ADD()
{
    printf("execute %x \n", opcode);
    u16 r = (V[(opcode >> 8) & 0x000F] + V[(opcode >> 4) & 0x000F]);
    if(r > 255)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
    V[(opcode >> 8) & 0x000F] = r;
}
/// @brief 8xy5 opcode
void Chip8::SUB()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0X000F] >= V[(opcode >> 4) & 0X000F]) 
        V[0xF] = 1; // there is a borrow
    else 
        V[0xF] = 0;					
    V[(opcode >> 8) & 0X000F] -= V[(opcode >> 4) & 0X000F];
}
/// @brief 8xy6 opcode
void Chip8::SHR()
{
    printf("execute %x \n", opcode);
    V[0xF] = (V[(opcode >> 8) & 0x000F]  & 0x1);
    V[(opcode >> 8) & 0x000F] >>= 1;
}
/// @brief 8xy7 opcode
void Chip8::SUBN()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 4) & 0X000F] >= V[(opcode >> 8) & 0X000F])	// VY-VX
        V[0xF] = 1; // there is a borrow
    else
        V[0xF] = 0;
    V[(opcode >> 8) & 0X000F] = V[(opcode >> 4) & 0X000F] - V[(opcode >> 8) & 0X000F];			
}
/// @brief 8xyE opcode
void Chip8::SHL()
{
    printf("execute %x \n", opcode);
    V[0xF] = V[(opcode >> 8) & 0x000F] >> 7;
    V[(opcode >> 8) & 0x000F] <<= 1;
}
/// @brief 9xy0 opcode
void Chip8::SNE_REG()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] != V[(opcode >> 4) & 0x000F])
        pc += 2;
}
/// @brief Annn opcode
void Chip8::LD_IADDR()
{
    printf("execute %x \n", opcode);
    I = opcode & 0x0FFF;
}
/// @brief Bnnn opcode
void Chip8::JMP_VADDR()
{
    printf("execute %x \n", opcode);
    pc -= 2;
    pc = (opcode & 0x0FFF) + V[0];
}
/// @brief Cxkk opcode
void Chip8::RND()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = ((rand() % 0xFF) & (opcode & 0x00FF));
}
/// @brief Dxyn opcode
void Chip8::DRW()
{
    printf("execute %x \n", opcode);
    u16 x = V[(opcode >> 8) & 0x000F];
    u16 y = V[(opcode >> 4) & 0x000F];
    u16 height = opcode & 0x000F;
    u16 pixel;
    
    V[0xF] = 0;
    for (int sizeUp = 0; sizeUp < height; sizeUp++)
    {
        pixel = memory[I + sizeUp];
        for(int sizeSide = 0; sizeSide < 8; sizeSide++)
        {
            if((pixel & (0x80 >> sizeSide)) != 0)
            {
                if(gfx[(x + sizeSide + ((y + sizeUp) * 64))] == 1)
                    V[0xF] = 1;                                 
                gfx[x + sizeSide + ((y + sizeUp) * 64)] ^= 1;
            }
        }
    }
    
    drawFlag = true;
}
/// @brief Ex9E opcode
void Chip8::SKP()
{
    printf("execute %x \n", opcode);
    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
        pc += 2;
}
/// @brief ExA1 opcode
void Chip8::SKNP()
{
    printf("execute %x \n", opcode);
    if(key[V[(opcode & 0x0F00) >> 8]] != 1)
        pc += 2;
}
/// @brief Fx07 opcode
void Chip8::LD_DT()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = delay_timer;
}
/// @brief Fx0A opcode
void Chip8::LD_K()
{
    printf("execute %x \n", opcode);
    for(int i = 0; i < 16; i++)
    {
        if(key[i] != 0)
        {
            V[(opcode >> 8) & 0x000F] = i;
            return;
        }
    }
    pc-=2;
}
/// @brief Fx15 opcode
void Chip8::LD_RDT()
{
    printf("execute %x \n", opcode);
    delay_timer = V[(opcode >> 8) & 0x000F];
}
/// @brief Fx18 opcode
void Chip8::LD_RST()
{
    printf("execute %x \n", opcode);
    sound_timer = V[(opcode >> 8) & 0x000F];
}
/// @brief Fx1E opcode
void Chip8::ADD_I()
{
    printf("execute %x \n", opcode);
    I = I + V[(opcode >> 8) & 0x000F];
    if(I > 0x0FFF)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0XF] = 0;
    }
}
/// @brief Fx29 opcode
void Chip8::LD_RF()
{
    printf("execute %x \n", opcode);
    I = V[(opcode >> 8) & 0x000F] * 5;
}
/// @brief Fx33 opcode
void Chip8::LD_RB()
{
    //printf("execute %x \n", opcode);
    memory[I] = (V[(opcode >> 8) & 0x000F]/100);
    memory[I+1] = ((V[(opcode >> 8) & 0x000F]/10)%10);
    memory[I+2] = ((V[(opcode >> 8) & 0x000F]%100)%10);
}
/// @brief Fx55 opcode
void Chip8::LD_IREG()
{
    for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
    {
        memory[I + i] = V[i];
    }
    I += ((opcode & 0x0F00) >> 8) + 1;
}
/// @brief Fx65 opcode
void Chip8::LD_RIREG()
{
    printf("execute %x \n", opcode);
    for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
    {
        V[i] = memory[I + i];
    }
    I += ((opcode & 0x0F00) >> 8) + 1;
}
