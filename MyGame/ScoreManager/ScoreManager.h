#pragma once
class ScoreManager
{
public: // 静的メンバ関数
	static ScoreManager* GetInstance();
public: // メンバ関数]
	const float& GetScore() { return score; }

public: // メンバ変数
	float score = 0.0f;
	float scoreMax = 9999999.0f;

	// ノーダメージフラグ
	bool noDamageFlag = true;
	// 目標スコアに達しているかフラグ
	bool targetScoreFlag = false;
	// 全ての部位を破壊したか
	bool allLegBreakFlag = false;
};