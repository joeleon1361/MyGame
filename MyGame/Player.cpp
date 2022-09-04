#include "Player.h"
#include "Input.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成 Instantiate a 3D object
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
	if (!Object3d::Initialize())
	{
		return false;
	}

    return true;
}

void Player::Update()
{
	// 移動処理
	//Move();

	// 移動制限
	//MoveLimit();

	// ロール
	//Rolling();

	// 回避
	// DodgeRoll();

	// 弾を発射
	Shot();

	Object3d::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	// オブジェクト移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			position.y += 0.3f;
		}
		else if (input->PushKey(DIK_S))
		{
			position.y -= 0.3f;
		}
		if (input->PushKey(DIK_D))
		{
			position.x += 0.3f;
		}
		else if (input->PushKey(DIK_A))
		{
			position.x -= 0.3f;
		}
	}
}

void Player::MoveLimit()
{
	// X軸を制限
	position.x = max(position.x, -MoveLimitX);
	position.x = min(position.x, +MoveLimitX);

	// Y軸を制限
	position.y = max(position.y, -MoveLimitY);
	position.y = min(position.y, +MoveLimitY);
}

void Player::Rolling()
{
	Input* input = Input::GetInstance();

	// ロール(横)
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

	// ロール(縦)
	/*if (input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		if (input->PushKey(DIK_S) && rotation.z <= +15.0f)
		{
			rotation.z += 3.0f;
		}

		if (input->PushKey(DIK_W) && rotation.z >= -15.0f)
		{
			rotation.z -= 3.0f;
		}
	}*/

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

	/*if (input->PushKey(DIK_W) == 0 && input->PushKey(DIK_S) == 0 && rotation.z != 0.0f)
	{
		if (rotation.z >= 0.0f)
		{
			rotation.z -= 3.0f;
		}

		if (rotation.z <= 0.0f)
		{
			rotation.z += 3.0f;
		}
	}*/
}

void Player::DodgeRoll()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_E) && DodgeTime >= 0)
	{
		if (DodgeTime >= 0)
		{
			position.x += 1.0;
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
