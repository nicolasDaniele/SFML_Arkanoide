#pragma once

#include <SFML/Audio.hpp>

class SoundManager
{
private:
    SoundManager() = default;
    
    bool load_sound(sf::SoundBuffer& buffer, sf::Sound& sound, 
        const std::string& path);
    
    sf::SoundBuffer boopBuffer;
    sf::SoundBuffer beepBuffer;
    sf::SoundBuffer clingBuffer;
    sf::SoundBuffer loseBuffer;

    sf::Sound boopSfx;
    sf::Sound beepSfx;
    sf::Sound clingSfx;
    sf::Sound loseSfx;

public:
    static SoundManager& get_instance()
    {
        static SoundManager instance;
        return instance;
    }

    bool init();
    void play_boop();
    void play_beep();
    void play_cling();
    void play_lose();
};