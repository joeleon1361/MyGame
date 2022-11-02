#include "Bullet.h"

Bullet* Bullet::Create(ObjModel* model, const XMFLOAT3 position)
{
	// 3Dオブジェクトのインスタンスを生成
	Bullet* instance = new Bullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Bullet::Initialize(const XMFLOAT3 position)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	return true;
}

void Bullet::Update()
{
	ObjObject::Update();

	// position.z += 2.0f;
}
