#include "BossParts.h"

using namespace DirectX;

// 生成
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

// 初期化
bool BossParts::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

// 更新
void BossParts::Update()
{
	ObjObject::Update();

	// ヒット時のカラー変更
	HitChangeColor();
}

// ヒット時のカラー変更
void BossParts::HitChangeColor()
{
	colorTimeRate += 0.1;
	if (colorTimeRate > 1.0f)
	{
		colorTimeRate = 1.0f;
	}
	color = Lerp::LerpFloat4(hitColor, baseColor, colorTimeRate);
}
