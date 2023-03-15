#pragma once
#include "ObjObject.h"

class StageObject :
	public ObjObject
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
// 3D�I�u�W�F�N�g����
	static std::unique_ptr<StageObject>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
// ������
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity);

	// ���t���[������
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // �����o�ϐ�
	float velocity = 0.0f;

public:
	// �f�X�t���O
	bool deathFlag = false;
};

