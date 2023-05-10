#pragma once

#include "DirectXCommon.h"
#include "ObjObject.h"
#include "GamePlay.h"
#include "Input.h"
#include "DebugText.h"

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

	// 描画
	void Draw() override;

	void DebugTextUpdate();

	void DebugTextDraw();

	void Move();

	// 移動制限
	void MoveLimit();

	void FrontRolling();

	void RightRolling();

	void BackRolling();

	void LeftRolling();

	// 回避
	void DodgeRoll();
	// 回避準備
	void judgeDodgeRoll();
	// 回避実行
	void executeDodgeRoll();

	// 弾を打つ
	void Shot();

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ObjModel* modelBullet = nullptr;
	DebugText debugText;

	// カメラ角度
	enum CAMERADIRECTION
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	// 移動制限
	const float LimitXZ = 18.0f;
	const float LimitY = 10.0f;

	// 正の速さ
	float playerPositiveSpeedX = {};
	float playerPositiveSpeedY = {};

	// 負の速さ
	float playerNegativeSpeedX = {};
	float playerNegativeSpeedY = {};

	// ロール角度
	XMFLOAT3 rollRotation = {};

	// 回避時の角度
	XMFLOAT3 dodgeRollRotation = {};
	// 回避前の角度
	XMFLOAT3 dodgeStartRotation = {};
	// 回避後の角度
	XMFLOAT3 dodgeEndRotation = {};

	// 回避時の速さ
	float dodgeRollSpeed = {};
	// 回避前の速さ
	float dodgeStartSpeed = {};
	// 回避後の速さ
	float dodgeEndSpeed = {};

	// 避用タイマー
	int32_t dodgeRollTimer = 0;

public:
	// 回避可能フラグ
	bool dodgeRollFlag = false;
	// 更新可能フラグ
	bool playerUpdateFlag = false;

	// プレイヤーの最大HP
	float maxHp = 300.0f;
	// プレイヤーのHP
	float nowHp = maxHp;
	// プレイヤーの最大HPから現HPを割った値
	float ratioHp = {};
};

