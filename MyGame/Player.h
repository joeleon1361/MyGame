#pragma once

#include "ObjObject.h"
#include "PlayerBullet.h"

class Player :
	public Object3d
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static Player* Create(Model* model = nullptr);

public: // メンバ関数
	// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// 移動処理
	void Move();

	// 移動制限
	void MoveLimit();

	// 弾を打つ
	void Shot();
	
private: // メンバ変数
	PlayerBullet* bullet = nullptr;

	const float MoveLimitX = 10;
	const float MoveLimitY = 5;
};

