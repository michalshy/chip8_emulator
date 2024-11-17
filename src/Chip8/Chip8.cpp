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

    //load sound
    vfx = LoadMusicStream("../resources/retro.mp3");
    UpdateMusicStream(vfx);
    PlayMusicStream(vfx);

    //timers
    sound_timer = 60;
    delay_timer = 60;

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
    //CLEAR KEYS
    for(int i = 0; i < 16; i++)
    {
        key[i] = 0;
    }

    drawFlag = true;
}

void Chip8::LoadGame()
{
    FILE *file;
    file = fopen("../games/pong.ch8", "rb");
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
            pc = stack[sp];
            --sp;
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
        if(V[(opcode >> 8) & 0x0F] == (opcode & 0x00FF))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x4000:
        if(V[(opcode >> 8) & 0x0f] != (opcode & 0x00FF))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x5000:
        if(V[(opcode >> 8) & 0x000F] == (V[(opcode >> 4) & 0x000F]))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x6000:
        V[(opcode >> 8) & 0x000F] = (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x7000:
        V[(opcode >> 8) & 0x000F] += (opcode & 0x00FF);
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            V[(opcode >> 8) & 0x000F] = V[(opcode >> 4) & 0x000F];
            pc += 2;
            break;
        case 0x0001:
            V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] | V[(opcode >> 4) & 0x000F]);
            pc += 2;
            break;
        case 0x0002:
            V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] & V[(opcode >> 4) & 0x000F]);
            pc += 2;
            break;
        case 0x0003:
            V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] ^ V[(opcode >> 4) & 0x000F]);
            pc += 2;
            break;
        case 0x0004:
            {
                u16 r = (V[(opcode >> 8) & 0x000F] + V[(opcode >> 4) & 0x000F]);
                if(r > 255)
                {
                    V[0xF] = 1;
                }
                else
                {
                    V[0xF] = 0;
                }
                V[(opcode >> 8) & 0x000F] = (r & 0x0000FFFF);
                pc += 2;
            }
            break;
        case 0x0005:
            if(V[(opcode >> 8) & 0x000F] > V[(opcode >> 4) & 0x000F])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x000F] = V[(opcode >> 8) & 0x000F] - V[(opcode >> 4) & 0x000F];
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
            if(V[(opcode >> 4) & 0x000F] > V[(opcode >> 8) & 0x000F])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[(opcode >> 8) & 0x000F] = V[(opcode >> 4) & 0x000F] - V[(opcode >> 8) & 0x000F];
            pc += 2;
            break;
        case 0x000E:
            V[0xF] = V[(opcode >> 8) & 0x000F] >> 7;
            V[(opcode >> 8) & 0x000F] <<= 1;
            pc += 2;
            break;
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);      
            break;    
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
        V[(opcode >> 8) & 0x0f] = (rand() & V[(opcode >> 4) & 0x0f]);
        pc += 2;
        break;
    case 0xD000:		   
        {
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
        pc += 2;
        }
        break;
    case 0xE000:
        switch(opcode & 0x00FF)
        {
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
                break;
        }
        break;
    case 0xF000:
        switch(opcode & 0x00FF)
        {
            // EX9E: Skips the next instruction 
            // if the key stored in VX is pressed
            case 0x0007:
                V[(opcode >> 8) & 0x000F] = delay_timer;
                pc+=2;
                break;
            case 0x000A:
                for(int i = 0; i < 16; i++)
                {
                    if(key[i] != 0)
                    {
                        pc+=2;
                        V[(opcode >> 8) & 0x000F] = key[i];
                        break;
                    }
                }
                break;
            case 0x0015:
                delay_timer = V[(opcode >> 8) & 0x000F];
                pc+=2;
                break;
            case 0x0018:
                sound_timer = V[(opcode >> 8) & 0x000F];
                pc+=2;
                break;
            case 0x001E:
                I = I + V[(opcode >> 8) & 0x000F];
                if(I > 0x0FFF)
                {
                    V[0xF] = 1;
                }
                pc+=2;
                break;
            case 0x0029:
                I = V[(opcode >> 8) & 0x000F] * 5;
                pc+=2;
                break;
            case 0x0033:
                memory[I] = (V[(opcode >> 8) & 0x000F]/100);
                memory[I+1] = ((V[(opcode >> 8) & 0x000F]/10)%10);
                memory[I+2] = ((V[(opcode >> 8) & 0x000F]%100)%10);
                pc+=2;
                break;
            case 0x0055:
                for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
                {
                    memory[I + i] = V[i];
                }
                pc+=2;
                break;
            case 0x0065:
                for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
                {
                    V[i] = memory[I + i];
                }
                pc+=2;
                break;
            default:
                break;
        }
        break;
    }
    // Update timers
    if(delay_timer > 0)
    {
        --delay_timer;
    }
    if(sound_timer > 0)
    {
        PlayMusicStream(vfx);
    }
    else
    {
        StopMusicStream(vfx);
    }
}

Chip8::~Chip8()
{
    UnloadMusicStream(vfx);
}