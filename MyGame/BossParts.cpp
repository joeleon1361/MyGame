#include "BossParts.h"

using namespace DirectX;

// ����
BossParts* BossParts::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossParts* instance = new BossParts();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

// ������
bool BossParts::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

// �X�V
void BossParts::Update()
{
	ObjObject::Update();

	// �q�b�g���̃J���[�ύX
	HitChangeColor();
}

// �q�b�g���̃J���[�ύX
void BossParts::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}
