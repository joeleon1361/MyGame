#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include <sstream>
#include <iomanip>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "ScoreText.h"

class Result : public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
// �X�v���C�g�̃e�N�X�`���ԍ�
	static enum TextureNumber
	{
		// ����
		common_dtxt_2,

		// ���U���g
		result_bg,
		result_frame_1,
		result_frame_2,
		result_frame_3,
		result_gtxt_1,
		result_gtxt_2,
		result_gtxt_3,
		result_gtxt_4,
		result_gtxt_5,
		result_gtxt_6,
		result_gtxt_7,
		result_gtxt_8,
		result_gtxt_9,
		result_gtxt_12,
		result_gtxt_13,
		result_gtxt_14,
		result_gtxt_15,
		result_parts_1,
		result_parts_2,
		result_parts_3,
		result_parts_4,
		result_parts_5,
		result_parts_6,

		// �G�t�F�N�g
		loading_effect_1
	};

public:
	// �R���X�g���N�^
	Result();

	// �f�X�g���N�^
	~Result();

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	// �e�N�X�`���̃��[�h
	void LoadTextureFunction();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();

	ScoreText scoreText;

	// �Q�[���V�[���p
	Camera* camera = nullptr;

	Sprite* LoadingBG = nullptr;

	Sprite* ResultBG = nullptr;
	Sprite* ResultBN_1 = nullptr;
	Sprite* ResultBN_2 = nullptr;
	Sprite* ResultBN_3 = nullptr;
	Sprite* resultGTXT_1 = nullptr;
	Sprite* resultGTXT_2 = nullptr;
	Sprite* resultGTXT_3 = nullptr;
	Sprite* resultGTXT_4 = nullptr;
	Sprite* resultGTXT_5 = nullptr;
	Sprite* resultGTXT_6 = nullptr;
	Sprite* resultGTXT_7 = nullptr;
	Sprite* resultGTXT_8 = nullptr;
	Sprite* resultGTXT_9 = nullptr;
	Sprite* resultGTXT_12 = nullptr;
	Sprite* resultGTXT_13 = nullptr;
	Sprite* resultGTXT_14 = nullptr;
	Sprite* resultGTXT_15 = nullptr;

	Sprite* resultParts_1 = nullptr;
	Sprite* resultParts_2 = nullptr;
	Sprite* resultParts_3 = nullptr;
	Sprite* resultParts_4 = nullptr;
	Sprite* resultParts_5 = nullptr;
	Sprite* resultParts_6 = nullptr;
	Sprite* resultParts_7 = nullptr;
	Sprite* resultParts_8 = nullptr;
	Sprite* resultParts_9 = nullptr;
	Sprite* resultParts_10 = nullptr;

	// �o�i�[1�̃T�C�Y
	XMFLOAT2 resultBN_1Size = {};
	// �o�i�[2�̃T�C�Y
	XMFLOAT2 resultBN_2Size = {};
	// �o�i�[3�̃T�C�Y
	XMFLOAT2 resultBN_3Size = {};


	XMFLOAT2 resultParts_1Size = {};
	XMFLOAT2 resultParts_2Size = {};
	XMFLOAT2 resultParts_3Size = {};

	// S�����N�}�[�N�̃T�C�Y
	XMFLOAT2 rankSSize = {};
	// A�����N�}�[�N�̃T�C�Y
	XMFLOAT2 rankASize = {};
	// B�����N�}�[�N�̃T�C�Y
	XMFLOAT2 rankBSize = {};
	// C�����N�}�[�N�̃T�C�Y
	XMFLOAT2 rankCSize = {};

	XMFLOAT2 resultGTXT_15Size = {};

	// �~�b�V�����B���X�^�[1�̃T�C�Y
	XMFLOAT2 missionStar1Size = {};
	// �~�b�V�����B���X�^�[2�̃T�C�Y
	XMFLOAT2 missionStar2Size = {};
	// �~�b�V�����B���X�^�[3�̃T�C�Y
	XMFLOAT2 missionStar3Size = {};

	// �X�R�A�e�L�X�g�̕\�����W
	XMFLOAT2 scorePosition = { 855.0f, 275.0f };
	// �m�[�_���[�W�{�[�i�X�e�L�X�g�̕\�����W
	XMFLOAT2 noDamageBonusPosition = { 855.0f, 305.0f };
	// ���v�X�R�A�e�L�X�g�̕\�����W
	XMFLOAT2 totalScorePosition = { 800.0f, 385.0f };
	// �N���A�����N�e�L�X�g�̕\�����W
	XMFLOAT2 rankPosition = { 800.0f, 455.0f };
	// �N���A�����N�}�[�N�̕\�����W
	XMFLOAT2 resultRankPosition = { 850.0f,480.0f };

	// ���U���g�e�L�X�g�̐F
	XMFLOAT4 resultColor = {};
	XMFLOAT4 resultPartsColor = {};
	// �X�R�A�e�L�X�g�̐F
	XMFLOAT4 scoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// �m�[�_���[�W�e�L�X�g�̐F
	XMFLOAT4 noDamageBonusColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// ���v�X�R�A�e�L�X�g�̐F
	XMFLOAT4 totalScoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// �N���A�����N�e�L�X�g�̐F
	XMFLOAT4 rankColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// �~�b�V�����e�L�X�g�̐F
	XMFLOAT4 missionColor = { 0.760f, 0.929f, 1.0f, 0.0f };

	// S�����N�}�[�N�̐F
	XMFLOAT4 rankSColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// A�����N�}�[�N�̐F
	XMFLOAT4 rankAColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// B�����N�}�[�N�̐F
	XMFLOAT4 rankBColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// C�����N�}�[�N�̐F
	XMFLOAT4 rankCColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	// �~�b�V����1�e�L�X�g�̐F
	XMFLOAT4 mission1Color = {};
	// �~�b�V����2�e�L�X�g�̐F
	XMFLOAT4 mission2Color = {};
	// �~�b�V����3�e�L�X�g�̐F
	XMFLOAT4 mission3Color = {};

	// �~�b�V�������B���X�^�[1�̐F
	XMFLOAT4 missionStar1Color = {};
	// �~�b�V�������B���X�^�[2�̐F
	XMFLOAT4 missionStar2Color = {};
	// �~�b�V�������B���X�^�[3�̐F
	XMFLOAT4 missionStar3Color = {};

	// �~�b�V�����B���X�^�[1�̐F
	XMFLOAT4 missionStar4Color = {};
	// �~�b�V�����B���X�^�[2�̐F
	XMFLOAT4 missionStar5Color = {};
	// �~�b�V�����B���X�^�[3�̐F
	XMFLOAT4 missionStar6Color = {};

	float resultMoveVelX_1 = 0.0f;
	float resultMoveAccX_1 = 0.0f;

	float resultMoveVelY_1 = 0.0f;
	float resultMoveAccY_1 = 0.0f;

	float resultMoveVelX_2 = 0.0f;
	float resultMoveAccX_2 = 0.0f;

	float resultMoveVelX_3 = 0.0f;
	float resultMoveAccX_3 = 0.0f;

	float resultMoveVelX_4 = 0.0f;
	float resultMoveAccX_4 = 0.0f;

	float resultMoveVelX_5 = 0.0f;
	float resultMoveAccX_5 = 0.0f;

	float resultMoveVelX_6 = 0.0f;
	float resultMoveAccX_6 = 0.0f;

	float resultMoveVel_7 = 0.0f;

	float resultMoveVel_8 = 0.0f;

	float resultMoveVel_9 = 0.0f;
	float resultMoveVel_10 = 0.0f;
	float resultMoveVel_11 = 0.0f;

	float resultChangeAlpha_1 = 0.0f;
	float resultChangeAlpha_2 = 0.0f;
	float resultChangeAlpha_3 = 0.0f;
	float resultChangeAlpha_4 = 0.0f;
	float resultChangeAlpha_5 = 0.0f;
	float resultChangeAlpha_6 = 0.0f;
	float resultChangeAlpha_7 = 0.0f;
	float resultChangeAlpha_8 = 0.0f;
	float resultChangeAlpha_9 = 0.0f;
	float resultChangeAlpha_10 = 0.0f;
	float resultChangeAlpha_11 = 0.0f;

	// ���[�f�B���O�̈Ó]�F
	XMFLOAT4 loadingColor;

	// �V�[���J�ڎ��̈Ó]�A���t�@�l
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

	// ���U���g��ʂɓ����Ă���̎��Ԃ��v�Z����^�C�}�[
	float resultTimer = 5.0f;

	// �L�[���͂��\���t���O
	bool canPushKeyFlag = false;

	// ���U���g�e�L�X�g�̃T�C�Y�v�Z���J�n����t���O
	bool changeResultSizeFlag = false;

	// �m�[�_���[�W���ɉ��Z���Z�����l
	float noDamageBonus = 20000.0f;

	// ���v�X�R�A�̒l
	float totalScore = 0.0f;
};

