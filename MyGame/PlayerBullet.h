#pragma once

#include "ObjObject.h"

// プレイヤーの弾
class PlayerBullet :
	public Object3d
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static PlayerBullet* Create(Model* model = nullptr);

public: // メンバ関数
	// 初期化
	bool Initialize() override;

	// 更新
	void Update() override;
};

