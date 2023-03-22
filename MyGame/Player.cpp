#include "Player.h"

extern int cameraMode;
extern bool playerUpdateFlag;

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

	rotation.x = rollRotation.x + dodgeRollRotation.x;

	if (cameraMode == FRONT)
	{
		if (playerUpdateFlag == true)
		{
			// �I�u�W�F�N�g�ړ�
			FrontMove();
		}

		// �ړ�����
		MoveLimitXY();

		// ���[�����O
		FrontRolling();
	}
	else if (cameraMode == RIGHT)
	{
		if (playerUpdateFlag == true)
		{
			// �I�u�W�F�N�g�ړ�
			RightMove();
		}

		// �ړ�����
		MoveLimitZY();

		// ���[�����O
		RightRolling();
	}
	else if (cameraMode == BACK)
	{
		if (playerUpdateFlag == true)
		{
			// �I�u�W�F�N�g�ړ�
			BackMove();
		}

		// �ړ�����
		MoveLimitXY();

		// ���[�����O
		BackRolling();
	}
	else if (cameraMode == LEFT)
	{
		if (playerUpdateFlag == true)
		{
			// �I�u�W�F�N�g�ړ�
			LeftMove();
		}

		// �ړ�����
		MoveLimitZY();

		// ���[�����O
		LeftRolling();
	}

	// ���
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
			position.y += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_D))
		{
			position.x += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_A))
		{
			position.x -= playerVelocity + dodgeRollSpeed.x;
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
			position.y += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_D))
		{
			position.z -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_A))
		{
			position.z += playerVelocity + dodgeRollSpeed.x;
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
			position.y += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_D))
		{
			position.x -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_A))
		{
			position.x += playerVelocity + dodgeRollSpeed.x;
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
			position.y += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_S))
		{
			position.y -= playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_D))
		{
			position.z += playerVelocity + dodgeRollSpeed.x;
		}
		if (input->PushKey(DIK_A))
		{
			position.z -= playerVelocity + dodgeRollSpeed.x;
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

	if (playerUpdateFlag == true)
	{
		// ���[��
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

		// �X����߂�
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
		// �X����߂�
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
// �E�������̎��@�̌X��
void Player::RightRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// �X����߂�
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
// ��������̎��@�̌X��
void Player::BackRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// ���[��
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

		// �X����߂�
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
		// �X����߂�
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
// ���������̎��@�̌X��
void Player::LeftRolling()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{
		// �X����߂�
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

// ���
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

// �����������Ă���Ή���Ɏg�p����l��ݒ� 
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
			dodgeStartSpeed.x = 0.4f;
		}
		else if (input->TriggerKey(DIK_V) && input->PushKey(DIK_A))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed.x = 0.4f;
		}
	}
	else if (cameraMode == BACK)
	{
		if (input->TriggerKey(DIK_V) && input->PushKey(DIK_A))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = 720.0f;
			dodgeStartSpeed.x = 0.4f;
		}
		else if (input->TriggerKey(DIK_V) && input->PushKey(DIK_D))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed.x = 0.4f;
		}
	}
}

// ��������s
void Player::executeDodgeRoll()
{
	//�^�C�}�[���X�V
	const float rollTime = 40;
	dodgeRollTimer++;
	const float time = dodgeRollTimer / rollTime;

	dodgeRollRotation = Easing::OutQuadFloat3(dodgeStartRotation, dodgeEndRotation, time);

	dodgeRollSpeed = Easing::OutQuadFloat3(dodgeStartSpeed, dodgeEndSpeed, time);

	if (dodgeRollTimer >= rollTime) {
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
