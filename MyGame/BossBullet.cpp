#include "BossBullet.h"

using namespace DirectX;

std::unique_ptr<BossBullet> BossBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3Dオブジェクトのインスタンスを生成
	BossBullet* instance = new BossBullet();
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

	return std::unique_ptr<BossBullet>(instance);
}

bool BossBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
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

void BossBullet::Update()
{
	ObjObject::Update();

	position.z -= velocity;
}
