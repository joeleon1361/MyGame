#pragma once

#include "ObjObject.h"

// �v���C���[�̒e
class PlayerBullet :
	public ObjObject3d
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// 3D�I�u�W�F�N�g����
	static PlayerBullet* Create(ObjModel* model = nullptr);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
	// ������
	bool Initialize() override;

	// �X�V
	void Update() override;


private: // �����o�ϐ�
};

