#include "PlayerBullet.h"

using namespace DirectX;

PlayerBullet* PlayerBullet::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成 Instantiate a 3D object
	PlayerBullet* instance = new PlayerBullet();
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

bool PlayerBullet::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
{
}
