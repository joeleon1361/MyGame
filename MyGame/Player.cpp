#include "Player.h"

extern int cameraMode;

using namespace DirectX;

Player* Player::Create(ObjModel* model)
{
	// 3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	if (cameraMode == FRONT)
	{
		// オブジェクト移動
		FrontMove();

		// 移動制限
		MoveLimitXY();

		// ローリング
		FrontRolling();
	}
	else if (cameraMode == RIGHT)
	{
		// オブジェクト移動
		RightMove();

		// 移動制限
		MoveLimitZY();

		// ローリング
		RightRolling();
	}
	else if (cameraMode == BACK)
	{
		// オブジェクト移動
		BackMove();

		// 移動制限
		MoveLimitXY();

		// ローリング
		BackRolling();
	}
	else if (cameraMode == LEFT)
	{
		// オブジェクト移動
		LeftMove();

		// 移動制限
		MoveLimitZY();

		// ローリング
		LeftRolling();
	}

	DodgeRoll();
}

// 前方向移動処理
void Player::FrontMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			position.y += playerVelocity + dodgeRollVelocity;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			position.x += playerVelocity + dodgeRollVelocity;
		}
		if (input->PushKey(DIK_A))
		{
			position.x -= playerVelocity + dodgeRollVelocity;
		}
	}
}
// 右方向移動処理
void Player::RightMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			position.y += playerVelocity;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			position.z -= playerVelocity;
		}
		if (input->PushKey(DIK_A))
		{
			position.z += playerVelocity;
		}
	}
}
// 後方向移動処理
void Player::BackMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			position.y += playerVelocity;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			position.x -= playerVelocity;
		}
		if (input->PushKey(DIK_A))
		{
			position.x += playerVelocity;
		}
	}
}
// 左方向移動処理
void Player::LeftMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			position.y += playerVelocity;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			position.z += playerVelocity;
		}
		if (input->PushKey(DIK_A))
		{
			position.z -= playerVelocity;
		}
	}
}
// XY座標の移動制限
void Player::MoveLimitXY()
{
	// X軸を制限
	position.x = max(position.x, -LimitXZ);
	position.x = min(position.x, +LimitXZ);

	// Y軸を制限
	position.y = max(position.y, -LimitY);
	position.y = min(position.y, +LimitY);
}
// ZY座標の移動制限
void Player::MoveLimitZY()
{
	// Z軸を制限
	position.z = max(position.z, -LimitXZ);
	position.z = min(position.z, +LimitXZ);

	// Y軸を制限
	position.y = max(position.y, -LimitY);
	position.y = min(position.y, +LimitY);
}
// 前方向時の自機の傾き
void Player::FrontRolling()
{
	Input* input = Input::GetInstance();

	// ロール
	if (dodgeRollFlag == 0)
	{
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_D) && rotation.x <= +40.0f)
			{
				rotation.x += 5.0f;
			}

			if (input->PushKey(DIK_A) && rotation.x >= -40.0f)
			{
				rotation.x -= 5.0f;
			}
		}
	}

	// 傾きを戻す
	if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rotation.x != 0.0f)
	{
		if (rotation.x >= 0.0f)
		{
			rotation.x -= 5.0f;
		}

		if (rotation.x <= 0.0f)
		{
			rotation.x += 5.0f;
		}
	}
}
// 右方向時の自機の傾き
void Player::RightRolling()
{
	Input* input = Input::GetInstance();

	// 傾きを戻す
	if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rotation.x != 0.0f)
	{
		if (rotation.x >= 0.0f)
		{
			rotation.x -= 5.0f;
		}

		if (rotation.x <= 0.0f)
		{
			rotation.x += 5.0f;
		}
	}
}
// 後方向時の自機の傾き
void Player::BackRolling()
{
	Input* input = Input::GetInstance();

	// ロール
	if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		if (input->PushKey(DIK_D) && rotation.x >= -40.0f)
		{
			rotation.x -= 5.0f;
		}

		if (input->PushKey(DIK_A) && rotation.x <= +40.0f)
		{
			rotation.x += 5.0f;
		}
	}

	// 傾きを戻す
	if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rotation.x != 0.0f)
	{
		if (rotation.x >= 0.0f)
		{
			rotation.x -= 5.0f;
		}

		if (rotation.x <= 0.0f)
		{
			rotation.x += 5.0f;
		}
	}
}
// 左方向時の自機の傾き
void Player::LeftRolling()
{
	Input* input = Input::GetInstance();

	// 傾きを戻す
	if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rotation.x != 0.0f)
	{
		if (rotation.x >= 0.0f)
		{
			rotation.x -= 5.0f;
		}

		if (rotation.x <= 0.0f)
		{
			rotation.x += 5.0f;
		}
	}
}
// 回避
void Player::DodgeRoll()
{
	Input* input = Input::GetInstance();

	if (dodgeRollFlag == 0)
	{
		if (input->TriggerKey(DIK_V))
		{
			dodgeRollFlag = 1;
			dodgeRollVelocity = 0.4;
			// dodgeRollRotation = 5.0;
		}
	}
	else if (dodgeRollFlag == 1)
	{
		dodgeRollVelocity -= 0.01f;
		if (dodgeRollVelocity <= 0)
		{
			dodgeRollVelocity = 0.0f;
			dodgeRollFlag = 0;
		}
	}
}

void Player::Shot()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{

	}
}
