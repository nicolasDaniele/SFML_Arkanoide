#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
	score = 0;
}

void ScoreManager::add_to_score(int addedScore)
{
	score += addedScore;
}

int ScoreManager::get_score()
{
	return score;
}

void ScoreManager::reset_score()
{
	score = 0;
}