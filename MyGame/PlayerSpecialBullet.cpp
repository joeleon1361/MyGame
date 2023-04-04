#include "PlayerSpecialBullet.h"

extern int cameraMode;

using namespace DirectX;

std::unique_ptr<PlayerSpecialBullet> PlayerSpecialBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3Dオブジェクトのインスタンスを生成
	PlayerSpecialBullet* instance = new PlayerSpecialBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale, velocity)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<PlayerSpecialBullet>(instance);
}

bool PlayerSpecialBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
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

void PlayerSpecialBullet::Update()
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
