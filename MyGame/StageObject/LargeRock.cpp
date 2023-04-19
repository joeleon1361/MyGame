#include "LargeRock.h"

std::unique_ptr<LargeRock> LargeRock::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LargeRock* instance = new LargeRock();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, scale, velocity)) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<LargeRock>(instance);
}

bool LargeRock::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	this->position = position;
	this->scale = scale;
	this->velocity = velocity;
	return true;
}

void LargeRock::Update()
{
	ObjObject::Update();
	position.z -= velocity;

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}
