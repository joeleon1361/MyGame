#include "PlayerBullet.h"

using namespace DirectX;

XMVECTOR PlayerBullet::velocity;

std::unique_ptr<PlayerBullet> PlayerBullet::Create(ObjModel* model, XMFLOAT3 pos, XMFLOAT3 rot, const XMVECTOR& vel)
{
	// 3Dオブジェクトのインスタンスを生成
	PlayerBullet* instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(pos, rot)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	velocity = vel;

	return std::unique_ptr<PlayerBullet>(instance);
}

bool PlayerBullet::Initialize(XMFLOAT3 pos, XMFLOAT3 rot)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	position = pos;

	rotation = rot;

	return true;
}

void PlayerBullet::Update()
{
	if (!velocitySet)
	{
		velocityV = velocity;
		velocitySet = true;
	}

	position.x += velocityV.m128_f32[0] / 4.0f;
	position.y += velocityV.m128_f32[1] / 4.0f;
	position.z += velocityV.m128_f32[2] / 4.0f;
}
