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

	// ������ Initialize
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g Set model
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
	Input* input = Input::GetInstance();

	// �I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_I) || input->PushKey(DIK_K) || input->PushKey(DIK_J) || input->PushKey(DIK_L))
	{
		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_I))
		{
			position.z += 1.0f;
		}
		else if (input->PushKey(DIK_K))
		{
			position.z -= 1.0f;
		}
		if (input->PushKey(DIK_L))
		{
			position.x += 1.0f;
		}
		else if (input->PushKey(DIK_J))
		{
			position.x -= 1.0f;
		}
	}

	Object3d::Update();
}
