#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <optional>

class SoundManager
{
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

private:
	SoundManager() = default;

	bool load_sound(sf::SoundBuffer& buffer, std::optional<sf::Sound>& sound,
		const std::string& path);

	sf::SoundBuffer boopBuffer;
	sf::SoundBuffer beepBuffer;
	sf::SoundBuffer clingBuffer;
	sf::SoundBuffer loseBuffer;

	std::optional<sf::Sound> boopSfx;
	std::optional<sf::Sound> beepSfx;
	std::optional<sf::Sound> clingSfx;
	std::optional<sf::Sound> loseSfx;
};