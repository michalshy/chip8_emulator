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
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;
    case 0x3000: //Skip id Vv == kk
        if(V[(opcode >> 8) & 0x000F] == (opcode & 0x00FF))
        {
            pc+=2;
        }
        pc += 2;
        break;
    case 0x4000:
        if(V[(opcode >> 8) & 0x000F] != (opcode & 0x00FF))
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
                V[(opcode >> 8) & 0x000F] = r;
                pc += 2;
            }
            break;
        case 0x0005: // 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            if(V[(opcode >> 8) & 0X000F] >= V[(opcode >> 4) & 0X000F]) 
                V[0xF] = 1; // there is a borrow
            else 
                V[0xF] = 0;					
            V[(opcode >> 8) & 0X000F] -= V[(opcode >> 4) & 0X000F];
            pc += 2;
            break;
        case 0x0006:
            V[0xF] = (V[(opcode >> 8) & 0x000F]  & 0x1);
            V[(opcode >> 8) & 0x000F] >>= 1;
            pc += 2;
            break;
        case 0x0007: // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            if(V[(opcode >> 4) & 0X000F] >= V[(opcode >> 8) & 0X000F])	// VY-VX
                V[0xF] = 1; // there is a borrow
            else
                V[0xF] = 0;
            V[(opcode >> 8) & 0X000F] = V[(opcode >> 4) & 0X000F] - V[(opcode >> 8) & 0X000F];				
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
        if(V[(opcode >> 8) & 0x000F] != V[(opcode >> 4) & 0x000F])
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
        V[(opcode >> 8) & 0x000F] = ((rand() % 0xFF) & (opcode & 0x00FF));
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
                        V[(opcode >> 8) & 0x000F] = i;
                        return;
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
                else
                {
                    V[0XF] = 0;
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
                // On the original interpreter, when the operation is done, I = I + X + 1.
                I += ((opcode & 0x0F00) >> 8) + 1;
                pc+=2;
                break;
            case 0x0065:
                for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
                {
                    V[i] = memory[I + i];
                }
                // On the original interpreter, when the operation is done, I = I + X + 1.
                I += ((opcode & 0x0F00) >> 8) + 1;
                pc+=2;
                break;
            default:
                break;
        }
        break;
    default:
        printf ("Unknown opcode: 0x%X\n", opcode);
        break;
    }
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

void Chip8::EmulateCycleSecond()
{
    drawFlag = false;

    // Fetch Opcode
    opcode = memory[pc] << 8 | memory[pc + 1];
    pc += 2;

    chip_func v = instructions[mainOpCodes[opcode & 0xF000]];
    (this->*v)();
    
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

void Chip8::ZEROS()
{
    switch (opcode & 0x000F)
    {
    case 0x0000: CLS(); break;
    case 0x000E: RET(); break;
    default: break;
    }
}

void Chip8::EIGHTS()
{
    switch (opcode & 0x000F)
    {
    case 0x0000: LD_REG(); break;
    case 0x0001: OR(); break;
    case 0x0002: AND(); break;
    case 0x0003: XOR(); break;
    case 0x0004: ADD(); break;
    case 0x0005: SUB(); break;
    case 0x0006: SHR(); break;
    case 0x0007: SUBN(); break;
    case 0x000E: SHL(); break;
    default: break;
    }
}

void Chip8::ES()
{
    switch (opcode & 0x00FF)
    {
    case 0x009E: SKP(); break;
    case 0x00A1: SKPN(); break;
    default: break;
    }
}

void Chip8::FS()
{
    switch (opcode & 0x00FF)
    {
    case 0x0007: LD_DT(); break;
    case 0x000A: LD_K(); break;
    case 0x0015: LD_RDT(); break;
    case 0x0018: LD_RST(); break;
    case 0x001E: ADD_I(); break;
    case 0x0029: LD_RF(); break;
    case 0x0033: LD_RB(); break;
    case 0x0055: LD_IREG(); break;
    case 0x0065: LD_RIREG(); break;
    default: break;
    }
}

void Chip8::CLS()
{
    printf("execute %x \n", opcode);
    for(int i = 0; i < (64 * 32); i++)
    {
        gfx[i] = 0;
    }
    drawFlag = true;
}

void Chip8::RET()
{
    printf("execute %x \n", opcode);
    --sp;
    pc = stack[sp];
}

void Chip8::JMP()
{
    printf("execute %x \n", opcode);
    pc = opcode & 0x0FFF;
}

void Chip8::CALL()
{
    printf("execute %x \n", opcode);
    stack[sp] = pc;
    ++sp;
    pc = opcode & 0x0FFF;
}

void Chip8::SE_BYTE()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] == (opcode & 0x00FF))
    {
        pc+=2;
    }
}

void Chip8::SNE_BYTE()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] != (opcode & 0x00FF))
    {
        pc+=2;
    }
}

void Chip8::SE_REG()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] == (V[(opcode >> 4) & 0x000F]))
    {
        pc+=2;
    }
}

void Chip8::LD_BYTE()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (opcode & 0x00FF);
}

void Chip8::ADD_BYTE()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] += (opcode & 0x00FF);
}

void Chip8::LD_REG()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = V[(opcode >> 4) & 0x000F];
}

void Chip8::OR()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] | V[(opcode >> 4) & 0x000F]);
}

void Chip8::AND()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] & V[(opcode >> 4) & 0x000F]);
}

void Chip8::XOR()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = (V[(opcode >> 8) & 0x000F] ^ V[(opcode >> 4) & 0x000F]);
}

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

void Chip8::SUB()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0X000F] >= V[(opcode >> 4) & 0X000F]) 
        V[0xF] = 1; // there is a borrow
    else 
        V[0xF] = 0;					
    V[(opcode >> 8) & 0X000F] -= V[(opcode >> 4) & 0X000F];
}

void Chip8::SHR()
{
    printf("execute %x \n", opcode);
    V[0xF] = (V[(opcode >> 8) & 0x000F]  & 0x1);
    V[(opcode >> 8) & 0x000F] >>= 1;
}

void Chip8::SUBN()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 4) & 0X000F] >= V[(opcode >> 8) & 0X000F])	// VY-VX
        V[0xF] = 1; // there is a borrow
    else
        V[0xF] = 0;
    V[(opcode >> 8) & 0X000F] = V[(opcode >> 4) & 0X000F] - V[(opcode >> 8) & 0X000F];			
}

void Chip8::SHL()
{
    printf("execute %x \n", opcode);
    V[0xF] = V[(opcode >> 8) & 0x000F] >> 7;
    V[(opcode >> 8) & 0x000F] <<= 1;
}

void Chip8::SNE_REG()
{
    printf("execute %x \n", opcode);
    if(V[(opcode >> 8) & 0x000F] != V[(opcode >> 4) & 0x000F])
        pc += 2;
}

void Chip8::LD_IADDR()
{
    printf("execute %x \n", opcode);
    I = opcode & 0x0FFF;
}

void Chip8::JMP_VADDR()
{
    printf("execute %x \n", opcode);
    pc -= 2;
    pc = (opcode & 0x0FFF) + V[0];
}

void Chip8::RND()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = ((rand() % 0xFF) & (opcode & 0x00FF));
}

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

void Chip8::SKP()
{
    printf("execute %x \n", opcode);
    if(key[V[(opcode & 0x0F00) >> 8]] != 0)
        pc += 2;
}

void Chip8::SKPN()
{
    printf("execute %x \n", opcode);
    if(key[V[(opcode & 0x0F00) >> 8]] != 1)
        pc += 2;
}

void Chip8::LD_DT()
{
    printf("execute %x \n", opcode);
    V[(opcode >> 8) & 0x000F] = delay_timer;
}

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

void Chip8::LD_RDT()
{
    printf("execute %x \n", opcode);
    delay_timer = V[(opcode >> 8) & 0x000F];
}

void Chip8::LD_RST()
{
    printf("execute %x \n", opcode);
    sound_timer = V[(opcode >> 8) & 0x000F];
}

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

void Chip8::LD_RF()
{
    printf("execute %x \n", opcode);
    I = V[(opcode >> 8) & 0x000F] * 5;
}

void Chip8::LD_RB()
{
    //printf("execute %x \n", opcode);
    memory[I] = (V[(opcode >> 8) & 0x000F]/100);
    memory[I+1] = ((V[(opcode >> 8) & 0x000F]/10)%10);
    memory[I+2] = ((V[(opcode >> 8) & 0x000F]%100)%10);
}

void Chip8::LD_IREG()
{
    for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
    {
        memory[I + i] = V[i];
    }
    I += ((opcode & 0x0F00) >> 8) + 1;
}

void Chip8::LD_RIREG()
{
    printf("execute %x \n", opcode);
    for(int i = 0; i <= ((opcode >> 8) & 0x000F); i++)
    {
        V[i] = memory[I + i];
    }
    I += ((opcode & 0x0F00) >> 8) + 1;
}
