#pragma once

#include "ObjObject.h"
#include "GameScene.h"
#include "Input.h"

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

	void FrontMove();

	void RightMove();

	void BackMove();

	void LeftMove();

	// 移動制限
	void MoveLimitXY();

	void MoveLimitZY();

	void FrontRolling();

	void RightRolling();

	void BackRolling();

	void LeftRolling();

	// 慣性計算
	void InertiaCalc();

	// 回避
	void DodgeRoll();

	void judgeDodgeRoll();

	void executeDodgeRoll();

	// 弾を打つ
	void Shot();

private: // メンバ変数
	ObjModel* modelBullet = nullptr;

	enum CAMERADIRECTION
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	const float LimitXZ = 18.0f;
	const float LimitY = 10.0f;

	float playerVelocity = 0.4f;
	float playerSpeedX = 0.0f;
	float playerSpeedY = 0.0f;

	XMFLOAT3 rollRotation = {};

	XMFLOAT3 dodgeRollRotation = {};
	XMFLOAT3 dodgeStartRotation = {};
	XMFLOAT3 dodgeEndRotation = {};

	float dodgeRollSpeed = {};
	float dodgeStartSpeed = {};
	float dodgeEndSpeed = {};

	//緊急回避用タイマー
	int32_t dodgeRollTimer = 0;

	//慣性用タイマー
	int32_t InertiaTimer = 0;

public:
	bool dodgeRollFlag = false;
};

