#include "PlayerBullet.h"

using namespace DirectX;

PlayerBullet* PlayerBullet::Create(ObjModel* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐� Instantiate a 3D object
	PlayerBullet* instance = new PlayerBullet();
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

bool PlayerBullet::Initialize()
{
	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void PlayerBullet::Update()
{
}
