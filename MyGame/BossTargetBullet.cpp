#include "BossTargetBullet.h"

std::unique_ptr<BossTargetBullet> BossTargetBullet::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed)
{
	// 3Dオブジェクトのインスタンスを生成
	BossTargetBullet* instance = new BossTargetBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(position, scale, target, speed)) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<BossTargetBullet>(instance);
}

bool BossTargetBullet::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const XMFLOAT3 target, const float speed)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	this->position = position;
	this->scale = scale;
	this->target = target;
	this->speed = speed;

	float magnitude = (float)sqrt((target.x - position.x) * (target.x - position.x) + (target.y - position.y) * (target.y - position.y) + (target.z - position.z) * (target.z - position.z));

	velocity.x = (target.x - position.x) / magnitude;
	velocity.y = (target.y - position.y) / magnitude;
	velocity.z = (target.z - position.z) / magnitude;

	return true;
}

void BossTargetBullet::Update()
{
	ObjObject::Update();

	position.x += velocity.x * speed;
	position.y += velocity.y * speed;
	position.z += velocity.z * speed;
}
