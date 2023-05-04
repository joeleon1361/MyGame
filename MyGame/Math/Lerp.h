#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
class Lerp
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
	static XMFLOAT2 LerpFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 LerpFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);

	static XMFLOAT4 LerpFloat4(const XMFLOAT4& start, const XMFLOAT4& end, const float time);
};

