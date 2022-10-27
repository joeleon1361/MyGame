#pragma once

#include "ObjObject.h"
#include "PlayerBullet.h"
#include "GameScreen.h"

class Bullet :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ変数
	// 3Dオブジェクト生成
	static Bullet* Create(ObjModel* model, XMFLOAT3 pos);

public: // メンバ関数
	// 初期化
	bool Initialize(XMFLOAT3 pos);

	// 毎フレーム処理
	void Update();

private: // メンバ変数
};

