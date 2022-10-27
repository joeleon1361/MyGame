#pragma once

#include "ObjObject.h"
#include "PlayerBullet.h"
#include "GameScreen.h"

class Bullet :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�ϐ�
	// 3D�I�u�W�F�N�g����
	static Bullet* Create(ObjModel* model, XMFLOAT3 pos);

public: // �����o�֐�
	// ������
	bool Initialize(XMFLOAT3 pos);

	// ���t���[������
	void Update();

private: // �����o�ϐ�
};

