#pragma once

#include "Camera.h"

#include <DirectXMath.h>

// ルートカメラ
class RouteCamera
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

private:
	// ワールド変換データ
	XMMATRIX matWorld;
	// ビュープロジェクション
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
};