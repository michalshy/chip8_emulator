#include "Chip8.hpp"
#include <cstdio>

void Chip8::Init()
{
    // Initialize registers and memory once
    pc      = 0x200;
    opcode  = 0;
    I       = 0;
    sp      = 0;
    for(int i = 0; i < 80; i++)
    {
        memory[i] = chip_fontset[i];
    }
}

void Chip8::LoadGame(char* path)
{
    FILE *file;
    file = fopen(path, "rb");
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
            break;
    
        case 0x000E: // 0x00EE: Returns from subroutine          
            pc = stack[sp];
            --sp;
            break;
    
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);          
        }
        break;
    case 0x2000:
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;
    case 0xA000: // ANNN: Sets I to the address NNN
        // Execute opcode
        I = opcode & 0x0FFF;
        pc += 2;
        break;
    case 0x8000:
        switch (opcode & 0x000F)
        {
        case 0x0004:
            /* code */
            break;
        
        default:
            break;
        }
    
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