#pragma once
class ScoreManager
{
public: // ƒƒ“ƒoŠÖ”
	void ScoreReset();

	void ScoreAdd(float addScore);

	const float& GetScore() { return score; }
private: // ƒƒ“ƒo•Ï”
	float score = 0.0f;
	float scoreMax = 9999999.0f;

	float scoreRate = 1.0f;
	int scoreRateCount = 0;

	float addScore = 0.0f;
};

