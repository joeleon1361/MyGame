#include "Planet.h"

Planet* Planet::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Planet* instance = new Planet();
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

bool Planet::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Planet::Update()
{
	ObjObject::Update();

	rotation.x += 0.1f;
	rotation.y += 0.1f;
}

void Planet::Draw()
{
	ObjObject::Draw();
}
