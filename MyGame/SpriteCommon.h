#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <cassert>

class SpriteCommon
{
public: // �����o�֐�
	// �C���X�^���X�擾
	static SpriteCommon* GetInstance();

	// ������
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

private: //�����o�ϐ�
	// �f�o�C�X
	ID3D12Device* device = nullptr;

	//�R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList = nullptr;
};

