#ifndef __AUDIOMANAGER_HPP
#define __AUDIOMANAGER_HPP

#include "raylib.h"
#include "../../Globals.hpp"

class AudioManager
{
    Music vfx;                                          ///< Music for playing
public:
    AudioManager() = delete;
    explicit AudioManager(const char* path);            ///< Default constructor
    ~AudioManager();
};

#endif //AudioManager.hpp