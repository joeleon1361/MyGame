#pragma once

#include "ObjObject.h"
#include "GameScreen.h"
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

	void DodgeRoll();

	// 弾を打つ
	void Shot();

private: // メンバ変数
	ObjModel* modelBullet = nullptr;

	enum CAMERAMODE
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	const float LimitXZ = 18.0f;
	const float LimitY = 10.0f;

	float playerVelocity = 0.3f;

	float dodgeRollVelocity = 0.0f;
	float dodgeRollRotation = 0.0f;
	int dodgeRollFlag = 0;
};

