#pragma once
#include "ObjObject.h"
class BossParts :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static BossParts* Create(ObjModel* model = nullptr);

private: // 静的メンバ変数


public: // メンバ関数
// 初期化
	bool Initialize() override;

	// 毎フレーム処理
	void Update() override;

private: // メンバ変数


public:
	// ボスパーツの最大HP
	float maxHp = 10.0f;
	// ボスパーツのHP
	float nowHp = maxHp;
	// ボスパーツの最大HPから現HPを割った値
	float ratioHp = {};

	// 撃破フラグ
	bool isBreak = false;

	// 生存フラグ
	bool isAlive = true;

	// デスフラグ
	bool isDeath = false;
};

