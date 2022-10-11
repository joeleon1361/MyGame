﻿#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "DebugText.h"
#include "Sound.h"
#include "FbxObject.h"
#include "ParticleManager.h"

class Player;

// ゲームシーン
class GameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	// コンストクラタ
	GameScene();

	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio);

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// カメラ移動
	void MoveCamera();

	// パーティクル生成
	void CreateParticles();

private:
	XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* spriteBG = nullptr;

	ParticleManager* particleMan = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelPlayer = nullptr;
	Model* modelBullet = nullptr;

	FbxModel* testmodel = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Player* objPlayer = nullptr;
	Object3d* objBullet = nullptr;
	Object3d* objCenter = nullptr;
	Object3d* objTest = nullptr;
	Object3d* objC = nullptr;
	Object3d* objtest1 = nullptr;
	Object3d* objtest2 = nullptr;
	Object3d* objtest3 = nullptr;
	Object3d* objtest4 = nullptr;

	FbxObject3d* testobject = nullptr;

	int ShotFlag;
	XMFLOAT3 Shot;

	int cameraMode = 0;

	XMFLOAT3 start = { 10.0f, 0.0f, 0.0f};
	XMFLOAT3 p2 = { -70.0f, 0.0f, -70.0f};
	XMFLOAT3 p3 = { -140.0f, 0.0f, 0.0f};
	XMFLOAT3 end = { -70.0f, 0.0f, 70.0f};

	std::vector<XMFLOAT3> checkPoint{ start, start, p2, p3, end,start, start };


	size_t startIndex = 1;

	// 時間計測に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;

	float maxTime = 2000.0f;
	float timeRate;
};

