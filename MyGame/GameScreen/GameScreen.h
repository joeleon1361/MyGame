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
#include "Camera.h"
#include "ObjModel.h"
#include "FbxLoader.h"
#include "FbxObject.h"

#include "Collider/SphereCollider.h"
#include "Collider/CollisionManager.h"
#include "Collider/Collision.h"

#include "Player.h"
#include "Bullet.h"
#include "Boss.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>

class CollisionManager;
class Player;
class Bullet;
class Boss;

// ゲームシーン
class GameScreen
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
	GameScreen();

	// デストラクタ
	~GameScreen();

	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Sound* audio);

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// タイトル
	void TitleUpdate();

	void TitleDraw();

	void TitleInitialize();

	// ゲーム
	void GameUpdate();

	void GameDraw();

	void GameInitialize();

	// リザルト
	void ResultUpdata();

	void ResultDraw();

	void ResultInitialize();

	// パーティクル生成
	void CreateParticles();

	void AllDebugText();

	void GameDebugText();
	//
	void CameraSwitching();

	void Attack();

	// 当たり判定
	bool OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB);
private:
	XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* audio = nullptr;
	DebugText debugText;
	CollisionManager* collisionManager = nullptr;

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* spriteBG = nullptr;
	Sprite* TitleBG = nullptr;
	Sprite* ResultBG = nullptr;

	ParticleManager* particleMan = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelGround = nullptr;
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelBullet = nullptr;

	FbxModel* testmodel = nullptr;

	ObjObject* objSkydome = nullptr;
	ObjObject* objGround = nullptr;
	
	// プレイヤー関連
	Player* player = nullptr;

	std::list<std::unique_ptr<Bullet>> bullets;

	// ボス関連
	Boss* bossBody = nullptr;
	Boss* bossLeg1 = nullptr;
	Boss* bossLeg2 = nullptr;
	Boss* bossLeg3 = nullptr;
	Boss* bossLeg4 = nullptr;

	ObjObject* objCenter = nullptr;

	ObjObject* objC = nullptr;

	FbxObject3d* testobject = nullptr;

	// カメラ関連
	enum CAMERAMODE
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

#pragma region スプライン曲線関連
	// スプライン曲線関連
	// プレイヤー制御点
	XMFLOAT3 start = { 10.0f, 0.0f, 0.0f };
	XMFLOAT3 p2 = { -70.0f, 0.0f, -70.0f };
	XMFLOAT3 p3 = { -140.0f, 0.0f, 0.0f };
	XMFLOAT3 end = { -70.0f, 0.0f, 70.0f };
	std::vector<XMFLOAT3> playerCheckPoint{ start, start, p2, p3, end,start, p2, p2 };

	// ボス制御点
	XMFLOAT3 Bstart = { 10.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp2 = { -70.0f, 0.0f, -50.0f };
	XMFLOAT3 Bp3 = { -150.0f, 0.0f, 20.0f };
	XMFLOAT3 Bend = { -70.0f, 0.0f, 90.0f };
	std::vector<XMFLOAT3> bossCheckPoint{ Bstart, Bstart, Bp2, Bp3, Bend,Bstart, Bp2, Bp2 };

	size_t startIndex = 1;

	// 時間計測に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;
	float elapsedTime;

	float maxTime = 8000.0f;
	float timeRate;
#pragma endregion

#pragma region プレイヤー関連
	// プレイヤー関連
	XMFLOAT3 playerPosition;
	XMFLOAT3 playerRotation;

	XMFLOAT3 playerBulletPosition;

	// 弾関連
	XMFLOAT3 bulletScale = { 0.3f, 0.3f, 0.3f };
	float bulletVelocity = 2.0f;
#pragma endregion

#pragma region ボス関連
	// ボス関連
	XMFLOAT3 bossPosition;
	XMFLOAT3 bossRotation;

	XMFLOAT3 bossLeg1Position;
	XMFLOAT3 bossLeg2Position;
	XMFLOAT3 bossLeg3Position;
	XMFLOAT3 bossLeg4Position;

	XMFLOAT3 bossLeg1WorldPosition;
	XMFLOAT3 bossLeg2WorldPosition;
	XMFLOAT3 bossLeg3WorldPosition;
	XMFLOAT3 bossLeg4WorldPosition;

	int bossHp = 10;
	int bossLeg1Hp = 10;
	int bossLeg2Hp = 10;
	int bossLeg3Hp = 10;
	int bossLeg4Hp = 10;

	int bossLeg1Break = false;
	int bossLeg2Break = false;
	int bossLeg3Break = false;
	int bossLeg4Break = false;

	int bossFlag = true;
	int bossLeg1Flag = true;
	int bossLeg2Flag = true;
	int bossLeg3Flag = true;
	int bossLeg4Flag = true;

	// デスフラグ
	bool bossDeathFlag = false;
#pragma endregion

	XMFLOAT3 centerPosition;

	XMFLOAT3 CameraPos;

	XMFLOAT3 SkydomPos;
	XMFLOAT3 SkydomRot;

	// シーン遷移関連
	enum SCENE
	{
		TITLE,
		GAME,
		RESULT
	};

	int scene = TITLE;
};