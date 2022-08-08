#pragma once

#include "ObjObject.h"
#include "PlayerBullet.h"

class Player :
	public Object3d
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static Player* Create(Model* model = nullptr);

public: // �����o�֐�
	// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

	// �ړ�����
	void Move();

	// �ړ�����
	void MoveLimit();

	void Rolling();

	void DodgeRoll();

	// �e��ł�
	void Shot();
	
private: // �����o�ϐ�
	PlayerBullet* bullet = nullptr;

	const float MoveLimitX = 8.5f;
	const float MoveLimitY = 5.0f;

	const float DodgeTime = 2.0f;
};

