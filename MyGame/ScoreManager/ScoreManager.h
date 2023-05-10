#pragma once
class ScoreManager
{
public: // �ÓI�����o�֐�
	static ScoreManager* GetInstance();
public: // �����o�֐�]
	const float& GetScore() { return score; }

public: // �����o�ϐ�
	float score = 0.0f;
	float scoreMax = 9999999.0f;

	// �m�[�_���[�W�t���O
	bool noDamageFlag = true;
	// �ڕW�X�R�A�ɒB���Ă��邩�t���O
	bool targetScoreFlag = false;
	// �S�Ă̕��ʂ�j�󂵂���
	bool allLegBreakFlag = false;
};