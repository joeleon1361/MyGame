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

	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/Sprite/Common/common_dtxt_1.png");
	// �f�o�b�O�e�L�X�g������
	debugText.Initialize(0);

	return true;
}

void Player::Update()
{
	ObjObject::Update();

	rotation.x = rollRotation.x + dodgeRollRotation.x;
	position.x += playerPositiveSpeedX + playerNegativeSpeedX + dodgeRollSpeed;
	position.y += playerPositiveSpeedY + playerNegativeSpeedY;
	//InertiaCalc();

	// �I�u�W�F�N�g�ړ�
	Move();

	// �ړ�����
	MoveLimit();

	if (cameraMode == FRONT)
	{
		// ���[�����O
		FrontRolling();
	}
	else if (cameraMode == RIGHT)
	{
		// ���[�����O
		RightRolling();
	}
	else if (cameraMode == BACK)
	{
		// ���[�����O
		BackRolling();
	}
	else if (cameraMode == LEFT)
	{
		// ���[�����O
		LeftRolling();
	}

	// ���
	DodgeRoll();

	DebugTextUpdate();
}

void Player::Draw()
{
	ObjObject::Draw();
}

void Player::DebugTextUpdate()
{
	// �v���C���[�̍��W��\��
	std::ostringstream PlayerPosition;
	PlayerPosition << "PlayerPosition:("
		<< std::fixed << std::setprecision(2)
		<< position.x << "," // x
		<< position.y << "," // y
		<< position.z << ") Local"; // z
	debugText.Print(PlayerPosition.str(), 10, 90, 1.0f);

	// �v���C���[�̊p�x��\��
	std::ostringstream PlayerRotation;
	PlayerRotation << "PlayerRotation:("
		<< std::fixed << std::setprecision(2)
		<< rotation.x << "," // x
		<< rotation.y << "," // y
		<< rotation.z << ")"; // z
	debugText.Print(PlayerRotation.str(), 10, 110, 1.0f);

	// �{�X��HP�֘A
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
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
}

// �O�����ړ�����
void Player::Move()
{
	Input* input = Input::GetInstance();

	if (playerUpdateFlag == true)
	{

		// �ړ���̍��W���v�Z
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

		// X���𐧌�
		playerPositiveSpeedX = max(playerPositiveSpeedX, 0.0f);
		playerPositiveSpeedX = min(playerPositiveSpeedX, 0.4f);

		playerNegativeSpeedX = max(playerNegativeSpeedX, -0.4f);
		playerNegativeSpeedX = min(playerNegativeSpeedX, 0.0f);

		// Y���𐧌�
		playerPositiveSpeedY = max(playerPositiveSpeedY, 0.0f);
		playerPositiveSpeedY = min(playerPositiveSpeedY, 0.4f);

		playerNegativeSpeedY = max(playerNegativeSpeedY, -0.4f);
		playerNegativeSpeedY = min(playerNegativeSpeedY, 0.0f);

		// �X����߂�
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

// XY���W�̈ړ�����
void Player::MoveLimit()
{
	// X���𐧌�
	position.x = max(position.x, -LimitXZ);
	position.x = min(position.x, +LimitXZ);

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
		if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
		{
			if (input->PushKey(DIK_RIGHT) && rollRotation.x <= +40.0f)
			{
				rollRotation.x += 5.0f;
			}

			if (input->PushKey(DIK_LEFT) && rollRotation.x >= -40.0f)
			{
				rollRotation.x -= 5.0f;
			}
		}

		// �X����߂�
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.x != 0.0f)
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
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.x != 0.0f)
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
		if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
		{
			if (input->PushKey(DIK_RIGHT) && rollRotation.x >= -40.0f)
			{
				rollRotation.x -= 5.0f;
			}

			if (input->PushKey(DIK_LEFT) && rollRotation.x <= +40.0f)
			{
				rollRotation.x += 5.0f;
			}
		}

		// �X����߂�
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.x != 0.0f)
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
		if (input->PushKey(DIK_LEFT) == 0 && input->PushKey(DIK_RIGHT) == 0 && rollRotation.x != 0.0f)
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
		if (input->TriggerKey(DIK_C) && input->PushKey(DIK_RIGHT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = 720.0f;
			dodgeStartSpeed = 0.4f;
		}
		else if (input->TriggerKey(DIK_C) && input->PushKey(DIK_LEFT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed = -0.4f;
		}
	}
	else if (cameraMode == BACK)
	{
		if (input->TriggerKey(DIK_C) && input->PushKey(DIK_LEFT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = 720.0f;
			dodgeStartSpeed = -0.4f;
		}
		else if (input->TriggerKey(DIK_C) && input->PushKey(DIK_RIGHT))
		{
			dodgeRollFlag = true;
			dodgeRollTimer = 0;
			dodgeEndRotation.x = -720.0f;
			dodgeStartSpeed = 0.4f;
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
