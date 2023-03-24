#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Easing
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
	// Quad(2乗)
	static float InQuadFloat(const float start, const float end, const float time);
	static float OutQuadFloat(const float start, const float end, const float time);
	static float InOutQuadFloat(const float start, const float end, const float time);

	static XMFLOAT2 InQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 OutQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 InOutQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 InQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 OutQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 InOutQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);

	// Cubic(3乗)
	static float InCubicFloat(const float start, const float end, const float time);
	static float OutCubicFloat(const float start, const float end, const float time);
	static float InOutCubicFloat(const float start, const float end, const float time);

	static XMFLOAT2 InCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 OutCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 InOutCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 InCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 OutCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 InOutCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);

	// Quart(4乗)
	static float InQuartFloat(const float start, const float end, const float time);
	static float OutQuartFloat(const float start, const float end, const float time);
	static float InOutQuartFloat(const float start, const float end, const float time);

	static XMFLOAT2 InQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 OutQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 InOutQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 InQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 OutQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 InOutQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);

	// Quint(5乗)
	static float InQuintFloat(const float start, const float end, const float time);
	static float OutQuintFloat(const float start, const float end, const float time);
	static float InOutQuintFloat(const float start, const float end, const float time);

	static XMFLOAT2 InQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 OutQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);
	static XMFLOAT2 InOutQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time);

	static XMFLOAT3 InQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 OutQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
	static XMFLOAT3 InOutQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time);
};

