#include "LivesManager.h"

void LivesManager::init()
{
	lives = maxLives;
}

void LivesManager::lose_life()
{
	lives--;
}

void LivesManager::add_life()
{
	lives++;
}

void LivesManager::reset_lives()
{
	lives = maxLives;
}

int LivesManager::get_lives() const
{
	return lives;
}

bool LivesManager::is_game_over() const
{
	return lives < 1;
}
