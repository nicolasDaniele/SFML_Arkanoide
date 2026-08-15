#include <iostream>
#include "SoundManager.h"

bool SoundManager::init()
{
    if (!load_sound(boopBuffer, boopSfx, "Assets/Sounds/boop.ogg"))
    {
        std::cout << "SoundManager: could not load boopSFX\n";
        return false;
    }
        
    if (!load_sound(beepBuffer, beepSfx, "Assets/Sounds/beep.ogg"))
    {
        std::cout << "SoundManager: could not load beepSfx\n";
        return false;
    }

    if (!load_sound(loseBuffer, loseSfx, "Assets/Sounds/lose.ogg"))
    {
        std::cout << "SoundManager: could not load loseSfx\n";
        return false;
    }

    return true;
}

bool SoundManager::load_sound(sf::SoundBuffer& buffer, sf::Sound& sound, const std::string& path)
{
    if (!buffer.loadFromFile(path))
    {
        std::cout << "Could not load sound: " << path << "\n";
        return false;
    }
    
    sound.setBuffer(buffer);
    return true;
}

void SoundManager::play_boop()
{
    boopSfx.play();
}

void SoundManager::play_beep()
{
    beepSfx.play();
}

void SoundManager::play_lose()
{
    loseSfx.play();
}