#pragma once
#include "ObjObject.h"

class Bullet :
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
	static Bullet* Create(ObjModel* model, const XMFLOAT3 position);

public: // メンバ関数
	// 初期化
	bool Initialize(const XMFLOAT3 position );

	// 毎フレーム処理
	void Update();

	// 座標の取得
	const XMFLOAT3& GetPosition() { return bulletPos; }

private: // メンバ変数
	
public:
	XMFLOAT3 bulletPos = { 0,0,0 };
};

