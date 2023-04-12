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

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// デバッグテキスト初期化
	debugText.Initialize(0);

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	rotation.x = rollRotation.x + dodgeRollRotation.x;
	position.x += playerSpeed.x + dodgeRollSpeed;
	position.y += playerSpeed.y;
	//InertiaCalc();

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

	std::ostringstream PlayerSpeed;
	PlayerSpeed << "PlayerSpeed:("
		<< std::fixed << std::setprecision(2)
		<< playerSpeed.x << "," // x
		<< playerSpeed.y << "," // y
		<< playerSpeed.z << ")"; // z
	debugText.Print(PlayerSpeed.str(), 10, 130, 1.0f);

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
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerSpeed.y += 0.05f;
			}
			if (input->PushKey(DIK_S))
			{
				playerSpeed.y -= 0.05f;
			}
			if (input->PushKey(DIK_D))
			{
				playerSpeed.x += 0.05f;
			}
			if (input->PushKey(DIK_A))
			{
				playerSpeed.x -= 0.05f;
			}
		}

		// X軸を制限
		playerSpeed.x = max(playerSpeed.x, -0.4f);
		playerSpeed.x = min(playerSpeed.x, +0.4f);

		// Y軸を制限
		playerSpeed.y = max(playerSpeed.y, -0.4f);
		playerSpeed.y = min(playerSpeed.y, +0.4f);

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
		if (playerSpeed.x > 0.0f && playerSpeed.x != 0.0f)
		{
			playerSpeed.x -= 0.02f;
		}

		if (playerSpeed.x < 0.0f && playerSpeed.x != 0.0f)
		{
			playerSpeed.x += 0.02f;
		}

		if (playerSpeed.y > 0.0f && playerSpeed.y != 0.0f)
		{
			playerSpeed.y -= 0.02f;
		}

		if (playerSpeed.y < 0.0f && playerSpeed.y != 0.0f)
		{
			playerSpeed.y += 0.02f;
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
			dodgeStartSpeed = -0.4f;
		}
		else if (input->TriggerKey(DIK_V) && input->PushKey(DIK_D))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
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
