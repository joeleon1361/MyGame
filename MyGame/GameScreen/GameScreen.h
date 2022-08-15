#pragma once

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
#include "Vector3.h"

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

	FbxObject3d* testobject = nullptr;

	int ShotFlag;
	XMFLOAT3 Shot;

	std::vector<Vector3> checkPoint;

	size_t startIndex = 1;

	// 時間計測に必要なデータ
	int startCount = 0;
	int adoCount = 1.0f;
	int nowCount = 0;
	int elapsedCount = 0;

	float maxTime = 5.0f;
	float timeRate;
};

