#pragma once
class ScoreManager
{
public: // メンバ関数
	void ScoreReset();

	void ScoreAdd(float addScore);

	const float& GetScore() { return score; }
private: // メンバ変数
	float score = 0.0f;
	float scoreMax = 9999999.0f;

	float scoreRate = 1.0f;
	int scoreRateCount = 0;

	float addScore = 0.0f;
};

