#include "Player.h"
#include "Input.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐� Instantiate a 3D object
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
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
	// �ړ�����
	//Move();

	// �ړ�����
	//MoveLimit();

	// ���[��
	//Rolling();

	// ���
	// DodgeRoll();

	// �e�𔭎�
	Shot();

	Object3d::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
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
	// X���𐧌�
	position.x = max(position.x, -MoveLimitX);
	position.x = min(position.x, +MoveLimitX);

	// Y���𐧌�
	position.y = max(position.y, -MoveLimitY);
	position.y = min(position.y, +MoveLimitY);
}

void Player::Rolling()
{
	Input* input = Input::GetInstance();

	// ���[��(��)
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

	// ���[��(�c)
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

	// �X����߂�
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
