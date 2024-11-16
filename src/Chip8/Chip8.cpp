#include "Chip8.hpp"
#include <cstdio>
#include <time.h>
#include <stdlib.h>

u8 chip8_fontset[80] =
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
    //init time
    srand(time(NULL)); 

    // Initialize registers and memory once
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;
    //CLEAR MEMORY
    for(int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }
    //LOAD FONTSET INTO MEMORY
    for(int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }
    //CLEAR DISPLAY
    for(int i = 0; i < (CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_WIDTH); i++)
    {
        gfx[i] = 0;
    }

    drawFlag = true;
}

void Chip8::LoadGame()
{
    FILE *file;
    file = fopen("../games/IBM.ch8", "rb");
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
            drawFlag = true;
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
    case 0x1000: //1NNN - JUMP
        pc = opcode & 0x0FFF;
        break;
    case 0x2000: //CALL SUBROUTING
        ++sp;
        stack[sp] = pc;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000: //Skip id Vv == kk
        if(V[(opcode >> 8) & 0x0f] == static_cast<unsigned char>(opcode))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x4000:
        if(V[(opcode >> 8) & 0x0f] != static_cast<unsigned char>(opcode))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x5000:
        if(V[(opcode >> 8) & 0x0f] == V[(opcode >> 4) & 0x0f])
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x6000:
        V[(opcode >> 8) & 0x000F] = (opcode & 0x00ff);
        pc += 2;
        break;
    case 0x7000:
        V[(opcode >> 8) & 0x000F] += (opcode & 0x00ff);
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0001:
            V[(opcode >> 8) & 0x0f] = (V[(opcode >> 8) & 0x0f] | V[(opcode >> 4) & 0x0f]);
            pc += 2;
            break;
        case 0x0002:
            V[(opcode >> 8) & 0x0f] = (V[(opcode >> 8) & 0x0f] & V[(opcode >> 4) & 0x0f]);
            pc += 2;
            break;
        case 0x0003:
            V[(opcode >> 8) & 0x0f] = (V[(opcode >> 8) & 0x0f] ^ V[(opcode >> 4) & 0x0f]);
            pc += 2;
            break;
        case 0x0004:
            if((V[(opcode >> 8) & 0x0f] + V[(opcode >> 4) & 0x0f]) > 255)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x0f] = V[(opcode >> 8) & 0x0f] + V[(opcode >> 4) & 0x0f];
            pc += 2;
            break;
        case 0x0005:
            if(V[(opcode >> 8) & 0x0f] > V[(opcode >> 4) & 0x0f])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x0f] -= V[(opcode >> 4) & 0x0f];
            pc += 2;
            break;
        case 0x0006:
            if((V[(opcode >> 8) & 0x0f] & 0b00000001) == 1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x0f] = V[(opcode >> 8) & 0x0f]/2;
            pc += 2;
            break;
        case 0x0007:
            if(V[(opcode >> 4) & 0x0f] > V[(opcode >> 8) & 0x0f])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 4) & 0x0f] -= V[(opcode >> 8) & 0x0f];
            pc += 2;
            break;
        case 0x000E:
            if((V[(opcode >> 8) & 0x0f] & 0b10000000) == 1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x0f] = V[(opcode >> 8) & 0x0f] * 2;
            pc += 2;
            break;
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
        }
        break;
    case 0x9000:
        if(V[(opcode >> 8) & 0x0f] != V[(opcode >> 4) & 0x0f])
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
        pc = (opcode & 0x0FFF) + V[0];
        break;
    case 0xC000:
        V[(opcode >> 8) & 0x0f] = rand() & V[(opcode >> 4) & 0x0f];
        pc += 2;
        break;
    case 0xD000:		   
        {
        u16 x = V[(opcode >> 8) & 0x000F];
        u16 y = V[(opcode >> 4) & 0x000F];
        u16 height = opcode & 0x000F;
        u16 pixel;
        
        V[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for(int xline = 0; xline < 8; xline++)
            {
            if((pixel & (0x80 >> xline)) != 0)
            {
                if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                V[0xF] = 1;                                 
                gfx[x + xline + ((y + yline) * 64)] ^= 1;
            }
            }
        }
        
        drawFlag = true;
        pc += 2;
        }
        break;
    case 0xE000:
        switch(opcode & 0x00FF)
        {
            // EX9E: Skips the next instruction 
            // if the key stored in VX is pressed
            case 0x009E:
                if(key[V[(opcode & 0x0F00) >> 8]] != 0)
                    pc += 4;
                else
                    pc += 2;
                break;
            case 0x00A1:
                if(key[V[(opcode & 0x0F00) >> 8]] != 1)
                    pc += 4;
                else
                    pc += 2;
                break;
        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
        }
    case 0xF000:
        switch(opcode & 0x00FF)
        {
            // EX9E: Skips the next instruction 
            // if the key stored in VX is pressed
            case 0x0007:
                V[(opcode & 0x0F00) >> 8] = delay_timer;
                pc+=2;
                break;
            case 0x000A:
                pc+=2;
                break;
            case 0x0015:
                pc+=2;
                break;
            case 0x0018:
                pc+=2;
                break;
            case 0x001E:
                pc+=2;
                break;
            case 0x0029:
                pc+=2;
                break;
            case 0x0033:
                pc+=2;
                break;
            case 0x0055:
                pc+=2;
                break;
            case 0x0065:
                pc+=2;
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
}