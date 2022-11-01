#pragma once
#include "ObjObject.h"

class Bullet :
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
	static Bullet* Create(ObjModel* model = nullptr);

public: // �����o�֐�
	// ������
	bool Initialize();

	// ���t���[������
	void Update();

private: // �����o�ϐ�
};

