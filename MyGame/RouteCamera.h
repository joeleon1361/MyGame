#pragma once

#include "Camera.h"

#include <DirectXMath.h>

// ���[�g�J����
class RouteCamera
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ������
	void Initialize();

	// �X�V
	void Update();

private:
	// ���[���h�ϊ��f�[�^
	XMMATRIX matWorld;
	// �r���[�v���W�F�N�V����
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
};