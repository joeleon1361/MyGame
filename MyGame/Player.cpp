#include "Player.h"

extern int cameraMode;

using namespace DirectX;

Player* Player::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
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
		// �I�u�W�F�N�g�ړ�
		FrontMove();

		// �ړ�����
		MoveLimitXY();

		// ���[�����O
		FrontRolling();
	}
	else if (cameraMode == RIGHT)
	{
		// �I�u�W�F�N�g�ړ�
		RightMove();

		// �ړ�����
		MoveLimitZY();

		// ���[�����O
		RightRolling();
	}
	else if (cameraMode == BACK)
	{
		// �I�u�W�F�N�g�ړ�
		BackMove();

		// �ړ�����
		MoveLimitXY();

		// ���[�����O
		BackRolling();
	}
	else if (cameraMode == LEFT)
	{
		// �I�u�W�F�N�g�ړ�
		LeftMove();

		// �ړ�����
		MoveLimitZY();

		// ���[�����O
		LeftRolling();
	}

	DodgeRoll();
}

// �O�����ړ�����
void Player::FrontMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
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
// �E�����ړ�����
void Player::RightMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
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
// ������ړ�����
void Player::BackMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
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
// �������ړ�����
void Player::LeftMove()
{
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// �ړ���̍��W���v�Z
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
// XY���W�̈ړ�����
void Player::MoveLimitXY()
{
	// X���𐧌�
	position.x = max(position.x, -LimitXZ);
	position.x = min(position.x, +LimitXZ);

	// Y���𐧌�
	position.y = max(position.y, -LimitY);
	position.y = min(position.y, +LimitY);
}
// ZY���W�̈ړ�����
void Player::MoveLimitZY()
{
	// Z���𐧌�
	position.z = max(position.z, -LimitXZ);
	position.z = min(position.z, +LimitXZ);

	// Y���𐧌�
	position.y = max(position.y, -LimitY);
	position.y = min(position.y, +LimitY);
}
// �O�������̎��@�̌X��
void Player::FrontRolling()
{
	Input* input = Input::GetInstance();

	// ���[��
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
}
// �E�������̎��@�̌X��
void Player::RightRolling()
{
	Input* input = Input::GetInstance();

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
}
// ��������̎��@�̌X��
void Player::BackRolling()
{
	Input* input = Input::GetInstance();

	// ���[��
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
}
// ���������̎��@�̌X��
void Player::LeftRolling()
{
	Input* input = Input::GetInstance();

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
}
// ���
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
