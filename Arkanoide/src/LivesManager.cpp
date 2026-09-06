#include "LivesManager.h"

void LivesManager::init()
{
	lives = startLives;
}

void LivesManager::lose_life()
{
	lives--;
}

void LivesManager::add_life()
{
	if (lives < maxLives)
	{
		lives++;
	}
}

void LivesManager::reset_lives()
{
	lives = startLives;
}

int LivesManager::get_lives() const
{
	return lives;
}

bool LivesManager::is_game_over() const
{
	return lives < 1;
}

bool LivesManager::is_at_max_lives() const
{
	return lives >= maxLives;
}