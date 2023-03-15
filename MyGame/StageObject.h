#pragma once
#include "ObjObject.h"

class StageObject :
	public ObjObject
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 静的メンバ関数
// 3Dオブジェクト生成
	static std::unique_ptr<StageObject>Create(ObjModel* model, const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity);

private: // 静的メンバ変数

public: // メンバ関数
// 初期化
	bool Initialize(const XMFLOAT3 position, const XMFLOAT3 scale, const float velocity);

	// 毎フレーム処理
	void Update();

	const bool GetDeathFlag() { return deathFlag; }

private: // メンバ変数
	float velocity = 0.0f;

public:
	// デスフラグ
	bool deathFlag = false;
};

