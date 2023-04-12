#include "Bullet.h"

extern int cameraMode;

using namespace DirectX;

std::unique_ptr<Bullet> Bullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Bullet* instance = new Bullet();
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

	return std::unique_ptr<Bullet>(instance);
}

bool Bullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
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

void Bullet::Update()
{
	ObjObject::Update();

	if (cameraMode == FRONT)
	{
		position.z += velocity;
	}
	else if (cameraMode == RIGHT)
	{
		position.x += velocity;
	}
	else if (cameraMode == BACK)
	{
		position.z -= velocity;
	}
	else if (cameraMode == LEFT)
	{
		position.x -= velocity;
	}

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}
