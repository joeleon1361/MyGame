#include "Planet.h"

Planet* Planet::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Planet* instance = new Planet();
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
