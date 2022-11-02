#include "Bullet.h"

std::unique_ptr<Bullet> Bullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	Bullet* instance = new Bullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<Bullet>(instance);
}

bool Bullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;
	return true;
}

void Bullet::Update()
{
	ObjObject::Update();

	position.z += Velocity;
}
