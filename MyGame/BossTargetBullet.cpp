#include "BossTargetBullet.h"

std::unique_ptr<BossTargetBullet> BossTargetBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 velocity)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossTargetBullet* instance = new BossTargetBullet();
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

	return std::unique_ptr<BossTargetBullet>(instance);
}

bool BossTargetBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 velocity)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;
	this->velocity = velocity;
	this->target = target;
	return true;
}

void BossTargetBullet::Update()
{
	ObjObject::Update();

	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
}
