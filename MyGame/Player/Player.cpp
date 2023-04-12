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
	position.x += playerSpeed.x + dodgeRollSpeed;
	position.y += playerSpeed.y;
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

	std::ostringstream PlayerSpeed;
	PlayerSpeed << "PlayerSpeed:("
		<< std::fixed << std::setprecision(2)
		<< playerSpeed.x << "," // x
		<< playerSpeed.y << "," // y
		<< playerSpeed.z << ")"; // z
	debugText.Print(PlayerSpeed.str(), 10, 130, 1.0f);

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
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// �ړ���̍��W���v�Z
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

		// X���𐧌�
		playerSpeed.x = max(playerSpeed.x, -0.4f);
		playerSpeed.x = min(playerSpeed.x, +0.4f);

		// Y���𐧌�
		playerSpeed.y = max(playerSpeed.y, -0.4f);
		playerSpeed.y = min(playerSpeed.y, +0.4f);

		// �X����߂�
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
		// �X����߂�
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
