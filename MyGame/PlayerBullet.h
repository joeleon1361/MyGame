#pragma once

#include "ObjObject.h"


// �v���C���[�̒e
class PlayerBullet :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<PlayerBullet>Create(ObjModel* model, XMFLOAT3 pos, XMFLOAT3 rot, const XMVECTOR& vel);

private: // �ÓI�����o�ϐ�
	static XMVECTOR velocity;

public: // �����o�֐�
	// ������
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 rot);

	// �X�V
	void Update();

private: // �����o�ϐ�
	XMVECTOR velocityV;
	bool velocitySet = false;
};

