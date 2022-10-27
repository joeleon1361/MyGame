#include "Bullet.h"

Bullet* Bullet::Create(ObjModel* model,XMFLOAT3 pos)
{
    // 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
    Bullet* instance = new Bullet();
    if (instance == nullptr)
	{
        return nullptr;
    }

	// ������
	if (!instance->Initialize(pos)) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Bullet::Initialize(XMFLOAT3 pos)
{
	position = pos;

	if (!ObjObject::Initialize())
	{
		return false;
	}

	return true;
}

void Bullet::Update()
{
	ObjObject::Update();

	//position.z += 0.1f;
}
