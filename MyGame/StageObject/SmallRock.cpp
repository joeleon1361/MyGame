#include "SmallRock.h"

std::unique_ptr<SmallRock> SmallRock::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3Dオブジェクトのインスタンスを生成
	SmallRock* instance = new SmallRock();
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

	return std::unique_ptr<SmallRock>(instance);
}

bool SmallRock::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
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

void SmallRock::Update()
{
	ObjObject::Update();
	position.z -= velocity;

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}
