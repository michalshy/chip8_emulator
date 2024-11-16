#ifndef __GLOBALS
#define __GLOBALS

#include "TypeDef.hpp"

constexpr u8 CHIP8_SCREEN_WIDTH = 64;
constexpr u8 CHIP8_SCREEN_HEIGHT = 32;

constexpr u16 SCREEN_WIDTH = CHIP8_SCREEN_WIDTH * 10;    static_assert(SCREEN_WIDTH == 640);
constexpr u16 SCREEN_HEIGHT = CHIP8_SCREEN_HEIGHT * 10;  static_assert(SCREEN_HEIGHT == 320);

constexpr u8 KEYS_LAYOUT[16] = 
{'1','2','3','4',
'q','w','e','r',
'a','s','d','f',
'z','x','c','v'};

constexpr char const* TITLE = "CHIP8_INTERPRETER";

#endif //__GLOBALS