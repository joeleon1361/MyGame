#pragma once

#include "DirectXCommon.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Spline
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

private: // �ÓI�����o�ϐ�

public: // �����o�֐�
	//XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

private: // �����o�ϐ�
	size_t startIndex = 1;

	// ���Ԍv���ɕK�v�ȃf�[�^
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;
	float elapsedTime;

	float maxTime = 2000.0f;
	float timeRate;
};

