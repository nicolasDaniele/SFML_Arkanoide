#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
private:
    SoundManager() { }
    ~SoundManager() { }
    
    bool load_sound(sf::SoundBuffer& buffer, sf::Sound& sound, 
        const std::string& path);
    
    static SoundManager* instance;

    sf::SoundBuffer boopBuffer;
    sf::SoundBuffer beepBuffer;
    sf::SoundBuffer loseBuffer;

    sf::Sound boopSfx;
    sf::Sound beepSfx;
    sf::Sound loseSfx;

public:
    static SoundManager* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new SoundManager();
        }
        return instance;
    }

    bool init();
    void play_boop();
    void play_beep();
    void play_lose();
};