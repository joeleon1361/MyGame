#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class BossParts :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
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

	// �q�b�g���̃J���[�ύX
	void HitChangeColor();

private: // �����o�ϐ�
	// �ʏ�̐F
	XMFLOAT4 baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// �q�b�g���̐F
	XMFLOAT4 hitColor = { 1.0f, 0.0f, 0.0f, 1.0f };

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

	// �J���[�ύX�^�C�����[�g
	float colorTimeRate = 1.0f;
};

