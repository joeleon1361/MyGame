#pragma once

#include "ObjObject.h"
#include "GameScreen.h"

class Player :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static Player* Create(ObjModel* model = nullptr);

public: // メンバ関数
	// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

	// 移動処理
	void FrontMove();

	void RightMove();

	void BackMove();

	void LeftMove();

	// 移動制限
	void MoveLimit();

	void Rolling();

	void DodgeRoll();

	// 弾を打つ
	void Shot();

private: // メンバ変数
	const float MoveLimitX = 8.5f;
	const float MoveLimitY = 5.0f;

	const float DodgeTime = 2.0f;
};

