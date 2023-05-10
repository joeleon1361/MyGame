#include "BossParts.h"

using namespace DirectX;

BossParts* BossParts::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	BossParts* instance = new BossParts();
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

bool BossParts::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void BossParts::Update()
{
	ObjObject::Update();
}
