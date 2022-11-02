#include "Bullet.h"

Bullet* Bullet::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Bullet* instance = new Bullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Bullet::Initialize()
{
	ObjObject::Initialize();
	return true;
}

void Bullet::Update()
{
	ObjObject::Update();

	position.z += 2.0f;
}
