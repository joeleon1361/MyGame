#pragma once

#include "ObjObject.h"

class Player :
	public Object3d
{
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 3Dオブジェクト生成 3D object generationd
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// 毎フレーム処理 Every frame processing
	/// </summary>
	void Update() override;
};

