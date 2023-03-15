#include "StageObject.h"

std::unique_ptr<StageObject> StageObject::Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	StageObject* instance = new StageObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(position, scale, velocity)) {
		delete instance;
		assert(0);
	}

	// ���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return std::unique_ptr<StageObject>(instance);
}

bool StageObject::Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity)
{
	if (!ObjObject::Initialize())
	{
		return false;
	}
	//SetRotation({ 0.0f,180.0f,0.0f });
	SetBillboard(true);

	this->position = position;
	this->scale = scale;
	this->velocity = velocity;
	return true;
}

void StageObject::Update()
{
	ObjObject::Update();
}
