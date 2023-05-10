#pragma once
#include "ObjObject.h"
class BossParts :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static BossParts* Create(ObjModel* model = nullptr);

private: // �ÓI�����o�ϐ�


public: // �����o�֐�
// ������
	bool Initialize() override;

	// ���t���[������
	void Update() override;

private: // �����o�ϐ�


public:
	// �{�X�p�[�c�̍ő�HP
	float maxHp = 10.0f;
	// �{�X�p�[�c��HP
	float nowHp = maxHp;
	// �{�X�p�[�c�̍ő�HP���猻HP���������l
	float ratioHp = {};

	// ���j�t���O
	bool isBreak = false;

	// �����t���O
	bool isAlive = true;

	// �f�X�t���O
	bool isDeath = false;
};

