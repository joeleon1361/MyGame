#pragma once

#include "ObjObject.h"
#include "GameScene.h"
#include "Input.h"

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

	void FrontMove();

	void RightMove();

	void BackMove();

	void LeftMove();

	// �ړ�����
	void MoveLimitXY();

	void MoveLimitZY();

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
	ObjModel* modelBullet = nullptr;

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

