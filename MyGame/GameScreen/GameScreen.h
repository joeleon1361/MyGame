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
	void CreateHitParticles(XMFLOAT3 position);

	void CreateBossParticles(XMFLOAT3 position);

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
	XMFLOAT3 start = { 0.0f, 0.0f, -70.0f };
	XMFLOAT3 p2 = { 0.0f, 0.0f, -60.0f };
	XMFLOAT3 p3 = { 0.0f, 0.0f, -50.0f };
	XMFLOAT3 p4 = { 0.0f, 0.0f, -40.0f };
	XMFLOAT3 p5 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 p6 = { 0.0f, 0.0f, -20.0f };
	XMFLOAT3 p7 = { 0.0f, 0.0f, -10.0f };
	XMFLOAT3 p8 = { 10.0f, 0.0f, 0.0f };
	XMFLOAT3 p9 = { 20.0f, 0.0f, 0.0f };
	XMFLOAT3 p10 = { 30.0f, 0.0f, 0.0f };
	XMFLOAT3 p11 = { 50.0f, 0.0f, 10.0f };
	XMFLOAT3 p12 = { 60.0f, 0.0f, 20.0f };
	XMFLOAT3 p13 = { 60.0f, 0.0f, 40.0f };
	XMFLOAT3 p14 = { 50.0f, 0.0f, 50.0f };
	XMFLOAT3 p15 = { 30.0f, 0.0f, 60.0f };
	XMFLOAT3 p16 = { 20.0f, 0.0f, 60.0f };
	XMFLOAT3 p17 = { 10.0f, 0.0f, 60.0f };
	XMFLOAT3 p18 = { 0.0f, 0.0f, 60.0f };
	XMFLOAT3 p19 = { -10.0f, 0.0f, 60.0f };
	XMFLOAT3 p20 = { -20.0f, 0.0f, 60.0f };
	XMFLOAT3 p21 = { -30.0f, 0.0f, 60.0f };
	XMFLOAT3 p22 = { -60.0f, 0.0f, 40.0f };
	XMFLOAT3 p23 = { -60.0f, 0.0f, 30.0f };
	XMFLOAT3 p24 = { -60.0f, 0.0f, 20.0f };
	XMFLOAT3 p25 = { -60.0f, 0.0f, 10.0f };
	XMFLOAT3 p26 = { -60.0f, 0.0f, 0.0f };
	XMFLOAT3 p27 = { -60.0f, 0.0f, -10.0f };
	XMFLOAT3 p28 = { -60.0f, 0.0f, -20.0f };
	XMFLOAT3 p29 = { -60.0f, 0.0f, -30.0f };
	XMFLOAT3 end = { -60.0f, 0.0f, -40.0f };
	std::vector<XMFLOAT3> playerCheckPoint{ start, start, p2, p3, p4, p5, p6, p7, p8, p9, p10,
		p11,p12, p13, p14, p15, p16, p17, p18, p19, p20, p21,p22, p23, p24, p25, p26, p27, p28,
	p29, end, end};

	// ボス制御点
	XMFLOAT3 Bstart = { 0.0f, 40.0f, -30.0f };
	XMFLOAT3 Bp2 = { 0.0f, 20.0f, -30.0f };
	XMFLOAT3 Bp3 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp4 = { 5.0f, 0.0f, -20.0f };
	XMFLOAT3 Bp5 = { 0.0f, -5.0f, -10.0f };
	XMFLOAT3 Bp6 = { -5.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp7 = { 0.0f, 5.0f, 10.0f };
	XMFLOAT3 Bp8 = { 10.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp9 = { 20.0f, -5.0f, 20.0f };
	XMFLOAT3 Bp10 = { 30.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp11 = { 40.0f, 5.0f, 20.0f };
	XMFLOAT3 Bp12 = { 40.0f, 10.0f, 20.0f };
	XMFLOAT3 Bp13 = { 40.0f, 10.0f, 40.0f };
	XMFLOAT3 Bp14 = { 40.0f, 5.0f, 40.0f };
	XMFLOAT3 Bp15 = { 30.0f, 0.0f, 40.0f };
	XMFLOAT3 Bp16 = { 20.0f, -5.0f, 40.0f };
	XMFLOAT3 Bp17 = { 10.0f, 0.0f, 40.0f };
	XMFLOAT3 Bp18 = { 0.0f, 5.0f, 40.0f };
	XMFLOAT3 Bp19 = { -10.0f, 0.0f, 40.0f };
	XMFLOAT3 Bp20 = { -20.0f, -5.0f, 40.0f };
	XMFLOAT3 Bp21 = { -30.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp22 = { -55.0f, 5.0f, 20.0f };
	XMFLOAT3 Bp23 = { -60.0f, 0.0f, 10.0f };
	XMFLOAT3 Bp24 = { -70.0f, -5.0f, 0.0f };
	XMFLOAT3 Bp25 = { -60.0f, 0.0f, -10.0f };
	XMFLOAT3 Bp26 = { -50.0f, 5.0f, -20.0f };
	XMFLOAT3 Bp27 = { -60.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp28 = { -65.0f, -5.0f, -40.0f };
	XMFLOAT3 Bp29 = { -60.0f, 0.0f, -50.0f };
	XMFLOAT3 Bend = { -60.0f, 5.0f, -60.0f };
	std::vector<XMFLOAT3> bossCheckPoint{ Bstart, Bstart, Bp2, Bp3, Bp4, Bp5, Bp6, Bp7, Bp8, Bp9, Bp10,
		Bp11,Bp12, Bp13, Bp14, Bp15, Bp16, Bp17, Bp18, Bp19, Bp20, Bp21,Bp22, Bp23, Bp24, Bp25, Bp26, Bp27, Bp28,
	Bp29, Bend, Bend };

	size_t startIndex = 1;

	// 時間計測に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;
	float elapsedTime;

	float maxTime = 2000.0f;
	float timeRate;
#pragma endregion

#pragma region プレイヤー関連
	// プレイヤー関連
	XMFLOAT3 playerPosition;
	XMFLOAT3 playerRotation;

	XMFLOAT3 playerWorldPosition;

	// 弾関連
	XMFLOAT3 bulletScale = { 0.3f, 0.3f, 0.3f };
	float bulletVelocity = 2.0f;

	float shotRate;
	bool shotFlag = true;
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

	int bossHp;
	int bossLeg1Hp;
	int bossLeg2Hp;
	int bossLeg3Hp;
	int bossLeg4Hp;

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