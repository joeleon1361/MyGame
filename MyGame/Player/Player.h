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

	void judgeDodgeRoll();

	void executeDodgeRoll();

	// �e��ł�
	void Shot();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ObjModel* modelBullet = nullptr;
	DebugText debugText;

	enum CAMERADIRECTION
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	const float LimitXZ = 18.0f;
	const float LimitY = 10.0f;

	float playerPositiveSpeedX = {};
	float playerPositiveSpeedY = {};

	float playerNegativeSpeedX = {};
	float playerNegativeSpeedY = {};

	XMFLOAT3 rollRotation = {};

	XMFLOAT3 dodgeRollRotation = {};
	XMFLOAT3 dodgeStartRotation = {};
	XMFLOAT3 dodgeEndRotation = {};

	float dodgeRollSpeed = {};
	float dodgeStartSpeed = {};
	float dodgeEndSpeed = {};

	//�ً}���p�^�C�}�[
	int32_t dodgeRollTimer = 0;

	//�����p�^�C�}�[
	int32_t InertiaTimer = 0;

public:
	bool dodgeRollFlag = false;

	bool playerUpdateFlag = false;
};

