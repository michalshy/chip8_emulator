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

/// @brief Section containg opcodes mapped to instructions array
/// Check Chip8.hpp private functions
static std::map<u16, u8> mappedMainOpcodes = { {0x0000, 0}, {0x1000, 1}, {0x2000, 2},
{0x3000, 3}, {0x4000, 4}, {0x5000, 5}, {0x6000, 6}, {0x7000, 7}, {0x8000, 8}, 
{0x9000, 9}, {0xA000, 10}, {0xB000, 11}, {0xC000, 12}, {0xD000, 13}, {0xE000, 14},
{0xF000, 15}};
static std::map<u16, u8> mappedZeroOpcodes = { {0x0000, 16}, {0x000E, 17} };
static std::map<u16, u8> mappedEightOpcodes = { {0x0000, 18}, {0x0001, 19}, {0x0002, 20},
{0x0003, 21}, {0x0004, 22}, {0x0005, 23}, {0x0006, 24}, {0x0007, 25}, {0x000E, 26} };
static std::map<u16, u8> mappedEsOpcodes = { {0x009E, 27}, {0x00A1, 28} };
static std::map<u16, u8> mappedFsOpcodes = { {0x0007, 29}, {0x000A, 30}, {0x0015, 31},
{0x0018, 32}, {0x001E, 33}, {0x0029, 34}, {0x0033, 35}, {0x0055, 36}, {0x0065, 37} };
/// End of section

constexpr u8 CHIP8_FONTSET[80] =
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

constexpr char const* MUSIC_PATH = "../resources/retro.wav";

constexpr char const* GAME = "../games/pong1.ch8";

#pragma endregion

#endif //__GLOBALS