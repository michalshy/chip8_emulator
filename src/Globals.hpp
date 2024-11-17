#ifndef __GLOBALS
#define __GLOBALS

#include "TypeDef.hpp"

/** 
 * CHIP-8 SECTION
 * Constants exclusive for CHIP-8 interpretes
 */
#pragma region Chip8Section
constexpr u8 CHIP8_SCREEN_WIDTH = 64;
constexpr u8 CHIP8_SCREEN_HEIGHT = 32;
#pragma endregion
/** 
 * SYSTEM SECTION
 * Constants provided for system, not interpreter
 */
#pragma region SystemSection
constexpr u16 SCREEN_WIDTH = CHIP8_SCREEN_WIDTH * 10;    static_assert(SCREEN_WIDTH == 640);
constexpr u16 SCREEN_HEIGHT = CHIP8_SCREEN_HEIGHT * 10;  static_assert(SCREEN_HEIGHT == 320);

constexpr u8 KEYS_LAYOUT[16] = 
{'1','2','3','4',
'q','w','e','r',
'a','s','d','f',
'z','x','c','v'};

constexpr char const* TITLE = "CHIP8_INTERPRETER";
#pragma endregion

#endif //__GLOBALS