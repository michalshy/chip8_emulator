#ifndef __WINDOWMANAGER_HPP
#define __WINDOWMANAGER_HPP

#include "raylib.h"
#include "../TypeDef.hpp"
#include "../Globals.hpp"
#include "../Chip8/Chip8.hpp"

class WindowManager
{
    Music sound;
public:
    explicit WindowManager() = default;
    void DrawChip(u8 * pixels);
    void CheckInput(u8 * key);
    void Loop(Chip8& chip);
};



#endif //__WINDOWMANAGER_HPP