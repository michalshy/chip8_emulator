#include "Chip8.hpp"
#include <cstdio>

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::Init()
{
    // Initialize registers and memory once
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;
    for(int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }
}

void Chip8::LoadGame()
{
    FILE *file;
    file = fopen("../games/glitchGhost.ch8", "rb");
    if(file == NULL)
    {
        perror("Error opening file!");
        return;
    }

    char c;
    int i = 0;
    while(fread(&c, sizeof(c), 1, file) == 1)
    {
        memory[i + 512] = c;
        i++;        
    }
}
char v, kk, x, y;
unsigned short res, loc;
void Chip8::EmulateCycle()
{
    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode Opcode
    switch (opcode & 0XF000)
    {
    case 0x0000:
        switch(opcode & 0x000F)
        {
        case 0x0000: // 0x00E0: Clears the screen        
            for(int i = 0; i < (64 * 32); i++)
            {
                gfx[i] = 0;
            }
            pc += 2;
            break;
    
        case 0x000E: // 0x00EE: Returns from subroutine          
            --sp;
            pc = stack[sp];
            pc += 2;
            break;
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
        }
        break;
    case 0x1000:
        pc = opcode & 0x0FFF;
        break;
    case 0x2000:
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000:
        v = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        kk = static_cast<unsigned char>(opcode);
        if(V[v] == kk)
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x4000:
        v = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        kk = static_cast<unsigned char>(opcode);
        if(V[v] != kk)
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x5000:
        x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
        if(V[y] == V[x])
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x6000:
        x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        kk = static_cast<unsigned char>(opcode);
        V[x] = kk;
        pc += 2;
        break;
    case 0x7000:
        x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        kk = static_cast<unsigned char>(opcode);
        V[x] += kk;
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0001:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            V[x] = (V[x] | V[y]);
            pc += 2;
            break;
        case 0x0002:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            V[x] = (V[x] & V[y]);
            pc += 2;
            break;
        case 0x0003:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            V[x] = (V[x] ^ V[y]);
            pc += 2;
            break;
        case 0x0004:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            res = static_cast<unsigned short>(V[x]) + static_cast<unsigned short>(V[y]);
            if(res > 255)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = static_cast<unsigned char>(res);
            pc += 2;
            break;
        case 0x0005:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            if(V[x] > V[y])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] -= V[y];
            pc += 2;
            break;
        case 0x0006:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            if((V[x] & 0b00000001) == 1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[x]/2;
            pc += 2;
            break;
        case 0x0007:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            if(V[y] > V[x])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[y] -= V[x];
            pc += 2;
            break;
        case 0x000E:
            x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
            y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
            if((V[x] & 0b10000000) == 1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[x] * 2;
            pc += 2;
            break;
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
        }
        break;
    case 0x9000:
        x = static_cast<unsigned char>((opcode >> 8) & 0x0f);
        y = static_cast<unsigned char>((opcode >> 4) & 0x0f);
        if(V[x] != V[y])
        {
            pc += 2;
        }
        pc += 2;
        break;
    case 0xA000: // ANNN: Sets I to the address NNN
        // Execute opcode
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case 0xB000:
        loc = opcode & 0x0FFF; 
        pc = loc + V[0];
        break;

    case 0xD000:		   
        {
            unsigned short X = V[(opcode & 0x0F00) >> 8];
            unsigned short Y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;

            V[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
            pixel = memory[I + yline];
            for(int xline = 0; xline < 8; xline++)
            {
                if((pixel & (0x80 >> xline)) != 0)
                {
                if(gfx[(X + xline + ((Y + yline) * 64))] == 1)
                    V[0xF] = 1;                                 
                gfx[X + xline + ((Y + yline) * 64)] ^= 1;
                }
            }
            }

            drawFlag = true;
            pc += 2;
        }
        break;
    default:
        printf ("Unknown opcode: 0x%X\n", opcode);
    }
   
    // Update timers
    if(delay_timer > 0)
    {
        --delay_timer;
    }
    if(sound_timer > 0)
    {
        if(sound_timer == 1)
        {
            printf("BEEP!\n");
        }
        --sound_timer;
    }
}