#include "ScoreManager.h"

void ScoreManager::init()
{
	score = 0;
}

void ScoreManager::add_to_score(int addedScore)
{
	score += addedScore;
}

int ScoreManager::get_score() const
{
	return score;
}

void ScoreManager::reset_score()
{
	score = 0;
}