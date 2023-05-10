#pragma once

#include "DirectXCommon.h"
#include "ObjObject.h"
#include "GamePlay.h"
#include "Input.h"
#include "DebugText.h"

class Player :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static Player* Create(ObjModel* model = nullptr);

public: // �����o�֐�
	// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

	// �`��
	void Draw() override;

	void DebugTextUpdate();

	void DebugTextDraw();

	void Move();

	// �ړ�����
	void MoveLimit();

	void FrontRolling();

	void RightRolling();

	void BackRolling();

	void LeftRolling();

	// ���
	void DodgeRoll();
	// �������
	void judgeDodgeRoll();
	// ������s
	void executeDodgeRoll();

	// �e��ł�
	void Shot();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ObjModel* modelBullet = nullptr;
	DebugText debugText;

	// �J�����p�x
	enum CAMERADIRECTION
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	// �ړ�����
	const float LimitXZ = 18.0f;
	const float LimitY = 10.0f;

	// ���̑���
	float playerPositiveSpeedX = {};
	float playerPositiveSpeedY = {};

	// ���̑���
	float playerNegativeSpeedX = {};
	float playerNegativeSpeedY = {};

	// ���[���p�x
	XMFLOAT3 rollRotation = {};

	// ������̊p�x
	XMFLOAT3 dodgeRollRotation = {};
	// ���O�̊p�x
	XMFLOAT3 dodgeStartRotation = {};
	// �����̊p�x
	XMFLOAT3 dodgeEndRotation = {};

	// ������̑���
	float dodgeRollSpeed = {};
	// ���O�̑���
	float dodgeStartSpeed = {};
	// �����̑���
	float dodgeEndSpeed = {};

	// ��p�^�C�}�[
	int32_t dodgeRollTimer = 0;

public:
	// ����\�t���O
	bool dodgeRollFlag = false;
	// �X�V�\�t���O
	bool playerUpdateFlag = false;

	// �v���C���[�̍ő�HP
	float maxHp = 300.0f;
	// �v���C���[��HP
	float nowHp = maxHp;
	// �v���C���[�̍ő�HP���猻HP���������l
	float ratioHp = {};
};

