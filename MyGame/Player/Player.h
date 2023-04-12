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

	// �����v�Z
	void InertiaCalc();

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

	float playerVelocity = 0.4f;
	float playerSpeedX = 0.0f;
	float playerSpeedY = 0.0f;

	XMFLOAT3 playerSpeed = {};

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
};

