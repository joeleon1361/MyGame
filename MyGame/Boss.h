#pragma once
#include "ObjObject.h"
class Boss :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static Boss* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数

public:
	// 弾の発射間隔
	static const int32_t ShotInterval = 60;

	static const int32_t RushInterval = 20;

public: // メンバ関数
// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

private: // メンバ変数
	

public:
	// 発射タイマー
	int32_t shotTimer = 0;
};

