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
	Move();

	// �ړ�����
	MoveLimit();

	// �e�𔭎�
	Shot();

	Object3d::Update();
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_I) || input->PushKey(DIK_K) || input->PushKey(DIK_J) || input->PushKey(DIK_L))
	{
		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_I))
		{
			position.y += 0.5f;
		}
		else if (input->PushKey(DIK_K))
		{
			position.y -= 0.5f;
		}
		if (input->PushKey(DIK_L))
		{
			position.x += 0.5f;
		}
		else if (input->PushKey(DIK_J))
		{
			position.x -= 0.5f;
		}
	}

	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_LEFT))
		{
			rotation.y -= 1.0f;
		}
		else if (input->PushKey(DIK_RIGHT))
		{
			rotation.y += 1.0f;
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

void Player::Shot()
{
	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_SPACE))
	{
	
	}
}
