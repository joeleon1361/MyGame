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

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	rotation.z = rollRotation.z + dodgeRollRotation.z;
	position.x += playerPositiveSpeedX + playerNegativeSpeedX + dodgeRollSpeed;
	position.y += playerPositiveSpeedY + playerNegativeSpeedY;

	// オブジェクト移動
	Move();

	// 移動制限
	MoveLimit();

	if (cameraMode == FRONT)
	{
		// ローリング
		FrontRolling();
	}
	else if (cameraMode == RIGHT)
	{
		// ローリング
		RightRolling();
	}
	else if (cameraMode == BACK)
	{
		// ローリング
		BackRolling();
	}
	else if (cameraMode == LEFT)
	{
		// ローリング
		LeftRolling();
	}

	// 回避
	DodgeRoll();

	DebugTextUpdate();
}

void Player::Draw()
{
	ObjObject::Draw();
}

void Player::DebugTextUpdate()
{
	// プレイヤーの座標を表示
	std::ostringstream PlayerPosition;
	PlayerPosition << "PlayerPosition:("
		<< std::fixed << std::setprecision(2)
		<< position.x << "," // x
		<< position.y << "," // y
		<< position.z << ") Local"; // z
	debugText.Print(PlayerPosition.str(), 10, 90, 1.0f);

	// プレイヤーの角度を表示
	std::ostringstream PlayerRotation;
	PlayerRotation << "PlayerRotation:("
		<< std::fixed << std::setprecision(2)
		<< rotation.x << "," // x
		<< rotation.y << "," // y
		<< rotation.z << ")"; // z
	debugText.Print(PlayerRotation.str(), 10, 110, 1.0f);

	// ボスのHP関連
	std::ostringstream DodgeRollSpeed;
	DodgeRollSpeed << "DodgeRollSpeed:("
		<< std::fixed << std::setprecision(2)
		<< dodgeRollSpeed << ")";
	debugText.Print(DodgeRollSpeed.str(), 10, 150, 1.0f);

	std::ostringstream RollRotation;
	RollRotation << "RollRotation:("
		<< std::fixed << std::setprecision(2)
		<< rollRotation.x << "," // x
		<< rollRotation.y << "," // y
		<< rollRotation.z << ")"; // z
	debugText.Print(RollRotation.str(), 10, 170, 1.0f);

	std::ostringstream DodgeRollRotation;
	DodgeRollRotation << "DodgeRollRotation:("
		<< std::fixed << std::setprecision(2)
		<< dodgeRollRotation.x << "," // x
		<< dodgeRollRotation.y << "," // y
		<< dodgeRollRotation.z << ")"; // z
	debugText.Print(DodgeRollRotation.str(), 10, 190, 1.0f);
}

void Player::DebugTextDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);
}

// 前方向移動処理
void Player::Move()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP))
		{
			playerPositiveSpeedY += 0.05f;
		}
		if (input->PushKey(DIK_DOWN))
		{
			playerNegativeSpeedY -= 0.05f;
		}
		if (input->PushKey(DIK_RIGHT))
		{
			playerPositiveSpeedX += 0.05f;
		}
		if (input->PushKey(DIK_LEFT))
		{
			playerNegativeSpeedX -= 0.05f;
		}

		// X軸を制限
		playerPositiveSpeedX = max(playerPositiveSpeedX, 0.0f);
		playerPositiveSpeedX = min(playerPositiveSpeedX, 0.4f);

		playerNegativeSpeedX = max(playerNegativeSpeedX, -0.4f);
		playerNegativeSpeedX = min(playerNegativeSpeedX, 0.0f);

		// Y軸を制限
		playerPositiveSpeedY = max(playerPositiveSpeedY, 0.0f);
		playerPositiveSpeedY = min(playerPositiveSpeedY, 0.4f);

		playerNegativeSpeedY = max(playerNegativeSpeedY, -0.4f);
		playerNegativeSpeedY = min(playerNegativeSpeedY, 0.0f);

		// 傾きを戻す
		if (input->PushKey(DIK_LEFT) == 0 && playerNegativeSpeedX != 0.0f)
		{
			playerNegativeSpeedX += 0.02f;
			if (playerNegativeSpeedX > 0.0f)
			{
				playerNegativeSpeedX = 0.0f;
			}
		}

		if (input->PushKey(DIK_RIGHT) == 0 && playerPositiveSpeedX != 0.0f)
		{
			playerPositiveSpeedX -= 0.02f;
			if (playerPositiveSpeedX < 0.0f)
			{
				playerPositiveSpeedX = 0.0f;
			}
		}

		if (input->PushKey(DIK_DOWN) == 0 && playerNegativeSpeedY != 0.0f)
		{
			playerNegativeSpeedY += 0.02f;
			if (playerNegativeSpeedY > 0.0f)
			{
				playerNegativeSpeedY = 0.0f;
			}
		}

		if (input->PushKey(DIK_UP) == 0 && playerPositiveSpeedY != 0.0f)
		{
			playerPositiveSpeedY -= 0.02f;
			if (playerPositiveSpeedY < 0.0f)
			{
				playerPositiveSpeedY = 0.0f;
			}
		}
	}
}

// XY座標の移動制限
void Player::MoveLimit()
{
	// X軸を制限
	position.x = max(position.x, -LimitXZ);
	position.x = min(position.x, +LimitXZ);

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
		if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
		{
			if (input->PushKey(DIK_LEFT) && rollRotation.z <= +40.0f)
			{
				rollRotation.z += 5.0f;
			}

			if (input->PushKey(DIK_RIGHT) && rollRotation.z >= -40.0f)
			{
				rollRotation.z -= 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
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
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
			}
		}
	}
	else
	{
		if (rollRotation.z > 0.0f)
		{
			rollRotation.z -= 5.0f;
		}

		if (rollRotation.z < 0.0f)
		{
			rollRotation.z += 5.0f;
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
		if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
		{
			if (input->PushKey(DIK_LEFT) && rollRotation.z >= -40.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (input->PushKey(DIK_RIGHT) && rollRotation.z <= +40.0f)
			{
				rollRotation.z += 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
			}
		}
	}
	else
	{
		// 傾きを戻す
		if (rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
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
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.z != 0.0f)
		{
			if (rollRotation.z > 0.0f)
			{
				rollRotation.z -= 5.0f;
			}

			if (rollRotation.z < 0.0f)
			{
				rollRotation.z += 5.0f;
			}
		}
	}
	else
	{
		if (rollRotation.z > 0.0f)
		{
			rollRotation.z -= 5.0f;
		}

		if (rollRotation.z < 0.0f)
		{
			rollRotation.z += 5.0f;
		}
	}

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
		if (input->TriggerKey(DIK_C) && input->PushKey(DIK_RIGHT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.z = -720.0f;
			dodgeStartSpeed = 0.4f;
		}
		else if (input->TriggerKey(DIK_C) && input->PushKey(DIK_LEFT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.z = 720.0f;
			dodgeStartSpeed = -0.4f;
		}
	}
	else if (cameraMode == BACK)
	{
		if (input->TriggerKey(DIK_C) && input->PushKey(DIK_LEFT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.z = -720.0f;
			dodgeStartSpeed = -0.4f;
		}
		else if (input->TriggerKey(DIK_C) && input->PushKey(DIK_RIGHT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.z = 720.0f;
			dodgeStartSpeed = 0.4f;
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
