#include "ScoreManager.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;

	return &instance;
}
