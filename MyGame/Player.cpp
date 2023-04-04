#include "Player.h"

extern int cameraMode;
extern bool playerUpdateFlag;

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

	rotation.x = rollRotation.x + dodgeRollRotation.x;
	position.x += playerSpeed.x;
	position.y += playerSpeed.y;
	//InertiaCalc();

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
		if (playerUpdateFlag == true)
		{
			// オブジェクト移動
			RightMove();
		}

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
		if (playerUpdateFlag == true)
		{
			// オブジェクト移動
			LeftMove();
		}

		// 移動制限
		MoveLimitZY();

		// ローリング
		LeftRolling();
	}

	// 回避
	DodgeRoll();
}

// 前方向移動処理
void Player::FrontMove()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W) && playerSpeed.y <= 0.4f)
			{
				playerSpeed.y += 0.05f;
			}
			if (input->PushKey(DIK_S) && playerSpeed.y >= -0.4f)
			{
				playerSpeed.y -= 0.05f;
			}
			if (input->PushKey(DIK_D) && playerSpeed.x <= 0.4f)
			{
				playerSpeed.x += 0.05f;
			}
			if (input->PushKey(DIK_A) && playerSpeed.x >= -0.4f)
			{
				playerSpeed.x -= 0.05f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerSpeed.x != 0.0f)
		{
			if (playerSpeed.x > 0.0f)
			{
				playerSpeed.x -= 0.02f;
			}

			if (playerSpeed.x < 0.0f)
			{
				playerSpeed.x += 0.02f;
			}
		}

		if (input->PushKey(DIK_W) == 0 && input->PushKey(DIK_S) == 0 && playerSpeed.y != 0.0f)
		{
			if (playerSpeed.y > 0.0f)
			{
				playerSpeed.y -= 0.02f;
			}

			if (playerSpeed.y < 0.0f)
			{
				playerSpeed.y += 0.02f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (playerSpeed.x != 0.0f)
		{
			if (playerSpeed.x > 0.0f)
			{
				playerSpeed.x -= 0.02f;
			}

			if (playerSpeed.x < 0.0f)
			{
				playerSpeed.x += 0.02f;
			}
		}

		if (playerSpeed.y != 0.0f)
		{
			if (playerSpeed.y > 0.0f)
			{
				playerSpeed.y -= 0.02f;
			}

			if (playerSpeed.y < 0.0f)
			{
				playerSpeed.y += 0.02f;
			}
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
			position.y += playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_D))
		{
			position.z -= playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_A))
		{
			position.z += playerVelocity + dodgeRollSpeed;
		}
	}
}
// 後方向移動処理
void Player::BackMove()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W) && playerSpeed.y <= 0.4f)
			{
				playerSpeed.y += 0.05f;
			}
			if (input->PushKey(DIK_S) && playerSpeed.y >= -0.4f)
			{
				playerSpeed.y -= 0.05f;
			}
			if (input->PushKey(DIK_A) && playerSpeed.x <= 0.4f)
			{
				playerSpeed.x += 0.05f;
			}
			if (input->PushKey(DIK_D) && playerSpeed.x >= -0.4f)
			{
				playerSpeed.x -= 0.05f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerSpeed.x != 0.0f)
		{
			if (playerSpeed.x > 0.0f)
			{
				playerSpeed.x -= 0.02f;
			}

			if (playerSpeed.x < 0.0f)
			{
				playerSpeed.x += 0.02f;
			}
		}

		if (input->PushKey(DIK_W) == 0 && input->PushKey(DIK_S) == 0 && playerSpeed.y != 0.0f)
		{
			if (playerSpeed.y > 0.0f)
			{
				playerSpeed.y -= 0.02f;
			}

			if (playerSpeed.y < 0.0f)
			{
				playerSpeed.y += 0.02f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (playerSpeed.x != 0.0f)
		{
			if (playerSpeed.x > 0.0f)
			{
				playerSpeed.x -= 0.02f;
			}

			if (playerSpeed.x < 0.0f)
			{
				playerSpeed.x += 0.02f;
			}
		}

		if (playerSpeed.y != 0.0f)
		{
			if (playerSpeed.y > 0.0f)
			{
				playerSpeed.y -= 0.02f;
			}

			if (playerSpeed.y < 0.0f)
			{
				playerSpeed.y += 0.02f;
			}
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
			position.y += playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_D))
		{
			position.z += playerVelocity + dodgeRollSpeed;
		}
		if (input->PushKey(DIK_A))
		{
			position.z -= playerVelocity + dodgeRollSpeed;
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

	if (playerUpdateFlag == true)
	{
		// ロール
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_D) && rollRotation.x <= +40.0f)
			{
				rollRotation.x += 5.0f;
			}

			if (input->PushKey(DIK_A) && rollRotation.x >= -40.0f)
			{
				rollRotation.x -= 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
}
// 右方向時の自機の傾き
void Player::RightRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
	else
	{
		if (rollRotation.x > 0.0f)
		{
			rollRotation.x -= 5.0f;
		}

		if (rollRotation.x < 0.0f)
		{
			rollRotation.x += 5.0f;
		}
	}

}
// 後方向時の自機の傾き
void Player::BackRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// ロール
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_D) && rollRotation.x >= -40.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (input->PushKey(DIK_A) && rollRotation.x <= +40.0f)
			{
				rollRotation.x += 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
}
// 左方向時の自機の傾き
void Player::LeftRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && rollRotation.x != 0.0f)
		{
			if (rollRotation.x > 0.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (rollRotation.x < 0.0f)
			{
				rollRotation.x += 5.0f;
			}
		}
	}
	else
	{
		if (rollRotation.x > 0.0f)
		{
			rollRotation.x -= 5.0f;
		}

		if (rollRotation.x < 0.0f)
		{
			rollRotation.x += 5.0f;
		}
	}

}

void Player::InertiaCalc()
{
	const float InertiaTime = 40;

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		InertiaTimer += 2.0f;
	}

	InertiaTimer--;

	if (InertiaTimer < 0)
	{
		InertiaTimer = 0;
	}
	if (InertiaTimer >= InertiaTime)
	{
		InertiaTimer = InertiaTime;
	}
	const float time = InertiaTimer / InertiaTime;
	playerVelocity = Easing::OutQuadFloat(0.0f, 0.4f, time);
}

// 回避
void Player::DodgeRoll()
{
	if (dodgeRollFlag == true)
	{
		executeDodgeRoll();
	}
	else
	{
		judgeDodgeRoll();
	}
}

// 条件が揃っていれば回避に使用する値を設定 
void Player::judgeDodgeRoll()
{
	Input* input = Input::GetInstance();

	if (cameraMode == FRONT)
	{
		if (input->TriggerKey(DIK_V) && input->PushKey(DIK_D))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = 720.0f;
			dodgeStartSpeed = 0.4f;
		}
		else if (input->TriggerKey(DIK_V) && input->PushKey(DIK_A))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed = -0.4f;
		}
	}
	else if (cameraMode == BACK)
	{
		if (input->TriggerKey(DIK_V) && input->PushKey(DIK_A))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = 720.0f;
			dodgeStartSpeed = 0.4f;
		}
		else if (input->TriggerKey(DIK_V) && input->PushKey(DIK_D))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed = -0.4f;
		}
	}
}

// 回避を実行
void Player::executeDodgeRoll()
{
	//タイマーを更新
	const float rollTime = 40;
	dodgeRollTimer++;
	const float time = dodgeRollTimer / rollTime;

	dodgeRollRotation = Easing::OutQuadFloat3(dodgeStartRotation, dodgeEndRotation, time);

	dodgeRollSpeed = Easing::OutQuadFloat(dodgeStartSpeed, dodgeEndSpeed, time);

	if (dodgeRollTimer >= rollTime)
	{
		dodgeRollFlag = false;
	}
}

void Player::Shot()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{

	}
}
