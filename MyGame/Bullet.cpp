#include "Bullet.h"

Bullet* Bullet::Create(ObjModel* model,XMFLOAT3 pos)
{
    // 3Dオブジェクトのインスタンスを生成
    Bullet* instance = new Bullet();
    if (instance == nullptr)
	{
        return nullptr;
    }

	// 初期化
	if (!instance->Initialize(pos)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Bullet::Initialize(XMFLOAT3 pos)
{
	position = pos;

	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Bullet::Update()
{
	ObjObject::Update();

	//position.z += 0.1f;
}
