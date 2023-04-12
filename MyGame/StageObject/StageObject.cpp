#include "StageObject.h"

std::unique_ptr<StageObject> StageObject::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3Dオブジェクトのインスタンスを生成
	StageObject* instance = new StageObject();
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

	return std::unique_ptr<StageObject>(instance);
}

bool StageObject::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
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

void StageObject::Update()
{
	ObjObject::Update();
	position.z -= velocity;

	if (--deathTimer <= 0)
	{
		deathFlag = true;
	}
}
