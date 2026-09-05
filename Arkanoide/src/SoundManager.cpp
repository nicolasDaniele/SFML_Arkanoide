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

	if (!load_sound(clingBuffer, clingSfx, "Assets/Sounds/cling.ogg"))
	{
		std::cout << "SoundManager: could not load clingSfx\n";
		return false;
	}

	if (!load_sound(loseBuffer, loseSfx, "Assets/Sounds/lose.ogg"))
	{
		std::cout << "SoundManager: could not load loseSfx\n";
		return false;
	}

	if (!load_sound(powerupBuffer, powerupSfx, "Assets/Sounds/powerup.ogg"))
	{
		std::cout << "SoundManager: could not load powerupSfx\n";
		return false;
	}

	return true;
}

bool SoundManager::load_sound(sf::SoundBuffer& buffer,
	std::optional<sf::Sound>& sound, const std::string& path)
{
	if (!buffer.loadFromFile(path))
	{
		std::cout << "Could not load sound: " << path << "\n";
		return false;
	}

	sound.emplace(buffer);
	sound->setVolume(SFX_VOLUME);

	return true;
}

void SoundManager::play_boop()
{
	if (boopSfx.has_value())
		boopSfx->play();
}

void SoundManager::play_beep()
{
	if (beepSfx.has_value())
		beepSfx->play();
}

void SoundManager::play_cling()
{
	if (clingSfx.has_value())
		clingSfx->play();
}

void SoundManager::play_lose()
{
	if (loseSfx.has_value())
		loseSfx->play();
}

void SoundManager::play_powerup()
{
	if (powerupSfx.has_value())
		powerupSfx->play();
}