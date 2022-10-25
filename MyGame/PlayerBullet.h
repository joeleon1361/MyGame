#pragma once

#include "ObjObject.h"


// プレイヤーの弾
class PlayerBullet :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // 静的メンバ関数
	// 3Dオブジェクト生成
	static std::unique_ptr<PlayerBullet>Create(ObjModel* model, XMFLOAT3 pos, XMFLOAT3 rot, const XMVECTOR& vel);

private: // 静的メンバ変数
	static XMVECTOR velocity;

public: // メンバ関数
	// 初期化
	bool Initialize(XMFLOAT3 pos, XMFLOAT3 rot);

	// 更新
	void Update();

private: // メンバ変数
	XMVECTOR velocityV;
	bool velocitySet = false;
};

