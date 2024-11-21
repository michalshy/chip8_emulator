#include "AudioManager.hpp"

AudioManager::AudioManager(const char* path)
{
    vfx = LoadMusicStream(path);
}



AudioManager::~AudioManager()
{
    UnloadMusicStream(vfx);
}
