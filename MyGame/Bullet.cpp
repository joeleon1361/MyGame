#include "Bullet.h"

Bullet* Bullet::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Bullet* instance = new Bullet();
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

bool Bullet::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Bullet::Update()
{
	ObjObject::Update();
}
