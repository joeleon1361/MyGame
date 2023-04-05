#include "ScoreManager.h"

void ScoreManager::ScoreReset()
{
	score = 0.0f;
}

void ScoreManager::ScoreAdd(float addScore)
{
	score += addScore;
}
