#pragma once
#include "ObjObject.h"
#include "Lerp.h"

class BossParts :
	public ObjObject
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
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

	// ヒット時のカラー変更
	void HitChangeColor();

private: // メンバ変数
	// 通常の色
	XMFLOAT4 baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// ヒット時の色
	XMFLOAT4 hitColor = { 1.0f, 0.0f, 0.0f, 1.0f };

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

	// カラー変更タイムレート
	float colorTimeRate = 1.0f;
};

