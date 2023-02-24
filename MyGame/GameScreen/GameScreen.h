#pragma once

#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "DebugText.h"
#include "ScoreText.h"
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
#include "BossBullet.h"
#include "BossTargetBullet.h"

#include "Spline.h"

#include "GamePad.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>

class CollisionManager;
class Player;
class Bullet;
class Boss;
class BossBullet;
class BossTargetBullet;

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

	static const int scoreTextTexNumber = 10;

public: // メンバ関数

	// コンストクラタ
	GameScreen();

	// デストラクタ
	~GameScreen();

	// 初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, Sound* sound);

	// 毎フレーム処理
	void Update();

	// 描画
	void Draw();

	// タイトル
	void TitleUpdate();

	void TitleDraw();

	void TitleInitialize();

	// ステージセレクト
	void StageSelectUpdate();

	void StageSelectDraw();

	void StageSelectInitialize();

	// ゲーム
	void GameUpdate();

	void GameDraw();

	void GameInitialize();

	// リザルト
	void ResultUpdate();

	void ResultDraw();

	void ResultInitialize();

	// ゲームオーバー
	void GameOverUpdate();

	void GameOverDraw();

	void GameOverInitialize();

	// パーティクル生成
	void CreateHitParticles(XMFLOAT3 position);

	void CreateBossParticles(XMFLOAT3 position);

	void AllDebugText();

	void TitleDebugText();

	void GameDebugText();
	//
	void CameraSwitching();

	void Attack();

	void BossAttack();

	void BossLeg1Attack();

	void BossLeg2Attack();

	void BossLeg3Attack();

	void BossLeg4Attack();

	void BossSpecialAttack();

	void SplineCount();

	void LoadTextureFunction();

	void LoadWavFunction();

	void scoreUIMotion();

	void scoreUIUpdate();

	// 当たり判定
	bool OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB);
private:
	XMFLOAT3 SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t);

	XMFLOAT3 lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t);

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	DebugText debugText;
	ScoreText scoreText;
	CollisionManager* collisionManager = nullptr;

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* spriteBG = nullptr;
	Sprite* TitleBG = nullptr;
	
	Sprite* StageSelectBG = nullptr;
	Sprite* GameFG = nullptr;
	Sprite* TitleStartUI = nullptr;

	Sprite* TitleLogo = nullptr;

	Sprite* LoadingBG = nullptr;

	// プレイヤーのHPUI
	Sprite* playerHpUI = nullptr;
	Sprite* playerHpGage = nullptr;
	Sprite* playerDamageGage = nullptr;
	Sprite* playerHpUICover = nullptr;

	// ボスのHPUI
	Sprite* bossHpUI = nullptr;
	Sprite* bossHpGage = nullptr;
	Sprite* bossDamageGage = nullptr;
	Sprite* bossHpUICover = nullptr;

	// ゲームUI
	Sprite* gameGTXT_1 = nullptr;
	Sprite* gameGTXT_2 = nullptr;
	Sprite* gameGTXT_3 = nullptr;
	Sprite* gameGTXT_4 = nullptr;

	// スコアUI
	Sprite* scoreUI = nullptr;
	Sprite* scoreChar = nullptr;
	Sprite* scoreNull_1 = nullptr;
	Sprite* scoreNull_2 = nullptr;
	Sprite* scoreNull_3 = nullptr;
	Sprite* scoreNull_4 = nullptr;
	Sprite* scoreNull_5 = nullptr;
	Sprite* scoreNull_6 = nullptr;

	// リザルトUI
	Sprite* ResultBG = nullptr;
	Sprite* ResultBN_1 = nullptr;
	Sprite* ResultBN_2 = nullptr;
	Sprite* ResultBN_3 = nullptr;
	Sprite* resultGTXT_1 = nullptr;
	Sprite* resultGTXT_2 = nullptr;
	Sprite* resultGTXT_3 = nullptr;
	Sprite* resultGTXT_4 = nullptr;
	Sprite* resultGTXT_5 = nullptr;
	Sprite* resultGTXT_6 = nullptr;
	Sprite* resultGTXT_7 = nullptr;
	Sprite* resultGTXT_8 = nullptr;
	Sprite* resultGTXT_9 = nullptr;
	Sprite* resultGTXT_12 = nullptr;
	Sprite* resultGTXT_13 = nullptr;
	Sprite* resultGTXT_14 = nullptr;
	Sprite* resultGTXT_15 = nullptr;

	Sprite* resultParts_1 = nullptr;
	Sprite* resultParts_2 = nullptr;
	Sprite* resultParts_3 = nullptr;
	Sprite* resultParts_4 = nullptr;
	Sprite* resultParts_5 = nullptr;
	Sprite* resultParts_6 = nullptr;
	Sprite* resultParts_7 = nullptr;
	Sprite* resultParts_8 = nullptr;
	Sprite* resultParts_9 = nullptr;
	Sprite* resultParts_10 = nullptr;

	Sprite* damageEffect = nullptr;

	ParticleManager* particleMan = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelGround = nullptr;
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelBullet = nullptr;
	ObjModel* modelBossLeg = nullptr;

	FbxModel* testmodel = nullptr;

	ObjObject* objSkydome = nullptr;
	ObjObject* objGround = nullptr;
	ObjObject* objTitlePlayer = nullptr;

	ObjObject* objStage1 = nullptr;
	ObjObject* objStage2 = nullptr;
	ObjObject* objStage3 = nullptr;

	// プレイヤー関連
	Player* player = nullptr;

	std::list<std::unique_ptr<Bullet>> bullets;

	// ボス関連
	Boss* bossBody = nullptr;
	Boss* bossLeg1 = nullptr;
	Boss* bossLeg2 = nullptr;
	Boss* bossLeg3 = nullptr;
	Boss* bossLeg4 = nullptr;

	std::list<std::unique_ptr<BossBullet>>bossBullets;

	std::list<std::unique_ptr<BossTargetBullet>>bossTargetBullets;

	ObjObject* objCenter = nullptr;

	ObjObject* frontCamera = nullptr;
	ObjObject* rightCamera = nullptr;
	ObjObject* backCamera = nullptr;
	ObjObject* leftCamera = nullptr;

	FbxObject3d* testobject = nullptr;

#pragma region カメラ関連
	enum CAMERAMODE
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	bool breakCameraFlag = false;
#pragma endregion

	bool railCountFlag = true;

#pragma region スプライン曲線関連
	// スプライン曲線関連
	// プレイヤー制御点
	/*XMFLOAT3 start = { 0.0f, 0.0f, -70.0f };
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
	XMFLOAT3 p30 = { -60.0f, 0.0f, -40.0f };
	XMFLOAT3 p31 = { -50.0f, 0.0f, -40.0f };
	XMFLOAT3 p32 = { -40.0f, 0.0f, -40.0f };
	XMFLOAT3 p33 = { -30.0f, 0.0f, -40.0f };
	XMFLOAT3 p34 = { -20.0f, 0.0f, -40.0f };
	XMFLOAT3 p35 = { -10.0f, 0.0f, -40.0f };
	XMFLOAT3 p36 = { 0.0f, 0.0f, -40.0f };
	XMFLOAT3 p37 = { 10.0f, 0.0f, -40.0f };
	XMFLOAT3 p38 = { 20.0f, 0.0f, -40.0f };
	XMFLOAT3 p39 = { 30.0f, 0.0f, -40.0f };
	XMFLOAT3 end = { 40.0f, 0.0f, -40.0f };
	XMFLOAT3 end2 = { 50.0f, 0.0f, -40.0f };*/

	XMFLOAT3 start = { 0.0f, 0.0f, -200.0f };
	XMFLOAT3 p2 = { 0.0f, 0.0f, -190.0f };
	XMFLOAT3 p3 = { 0.0f, 0.0f, -180.0f };
	XMFLOAT3 p4 = { 0.0f, 0.0f, -170.0f };
	XMFLOAT3 p5 = { 0.0f, 0.0f, -160.0f };
	XMFLOAT3 p6 = { 0.0f, 0.0f, -150.0f };
	XMFLOAT3 p7 = { 0.0f, 0.0f, -140.0f };
	XMFLOAT3 p8 = { 0.0f, 0.0f, -130.0f };
	XMFLOAT3 p9 = { 0.0f, 0.0f, -120.0f };
	XMFLOAT3 p10 = { 0.0f, 0.0f, -110.0f };
	XMFLOAT3 p11 = { 0.0f, 0.0f, -90.0f };
	XMFLOAT3 p12 = { 0.0f, 0.0f, -80.0f };
	XMFLOAT3 p13 = { 0.0f, 0.0f, -70.0f };
	XMFLOAT3 p14 = { 0.0f, 0.0f, -60.0f };
	XMFLOAT3 p15 = { 0.0f, 0.0f, -50.0f };
	XMFLOAT3 p16 = { 0.0f, 0.0f, -40.0f };
	XMFLOAT3 p17 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 p18 = { 0.0f, 0.0f, -20.0f };
	XMFLOAT3 p19 = { 0.0f, 0.0f, -10.0f };
	XMFLOAT3 p20 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 p21 = { 0.0f, 0.0f, 10.0f };
	XMFLOAT3 p22 = { 0.0f, 0.0f, 20.0f };
	XMFLOAT3 p23 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 p24 = { 0.0f, 0.0f, 40.0f };
	XMFLOAT3 p25 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 p26 = { 0.0f, 0.0f, 60.0f };
	XMFLOAT3 p27 = { 0.0f, 0.0f, 70.0f };
	XMFLOAT3 p28 = { 0.0f, 0.0f, 80.0f };
	XMFLOAT3 p29 = { 0.0f, 0.0f, 90.0f };
	XMFLOAT3 p30 = { 0.0f, 0.0f, 100.0f };
	XMFLOAT3 p31 = { 0.0f, 0.0f, 110.0f };
	XMFLOAT3 p32 = { 0.0f, 0.0f, 120.0f };
	XMFLOAT3 p33 = { 0.0f, 0.0f, 130.0f };
	XMFLOAT3 p34 = { 0.0f, 0.0f, 140.0f };
	XMFLOAT3 p35 = { 0.0f, 0.0f, 150.0f };
	XMFLOAT3 p36 = { 0.0f, 0.0f, 160.0f };
	XMFLOAT3 p37 = { 0.0f, 0.0f, 170.0f };
	XMFLOAT3 p38 = { 0.0f, 0.0f, 180.0f };
	XMFLOAT3 p39 = { 0.0f, 0.0f, 190.0f };
	XMFLOAT3 end = { 0.0f, 0.0f, 200.0f };
	XMFLOAT3 end2 = { 0.0f, 0.0f, 210.0f };

	std::vector<XMFLOAT3> playerCheckPoint{ start, start, p2, p3, p4, p5, p6, p7, p8, p9, p10,
		p11,p12, p13, p14, p15, p16, p17, p18, p19, p20, p21,p22, p23, p24, p25, p26, p27, p28,
	p29,p30,p31,p32, p33, p34, p35, p36, p37, p38,p39, end, end };

	std::vector<XMFLOAT3> playerTargetCheckPoint{ p2, p2, p3, p4, p5, p6, p7, p8, p9, p10,
		p11,p12, p13, p14, p15, p16, p17, p18, p19, p20, p21,p22, p23, p24, p25, p26, p27, p28,
	p29,p30,p31,p32, p33, p34, p35, p36, p37, p38,p39, end, end2, end2 };

	// ボス制御点
	/*XMFLOAT3 Bstart = { 0.0f, 40.0f, -30.0f };
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
	XMFLOAT3 Bp28 = { -65.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp29 = { -60.0f, 15.0f, -40.0f };
	XMFLOAT3 Bp30 = { -50.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp31 = { -40.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp32 = { -30.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp33 = { -20.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp34 = { -10.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp35 = { 0.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp36 = { 10.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp37 = { 20.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp38 = { 30.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp39 = { 40.0f, 0.0f, -40.0f };
	XMFLOAT3 Bend = { 50.0f, 0.0f, -40.0f };*/

	XMFLOAT3 Bstart = { 0.0f, 40.0f, -170.0f };
	XMFLOAT3 Bp2 = { 0.0f, 40.0f, -160.0f };
	XMFLOAT3 Bp3 = { 0.0f, 20.0f, -150.0f };
	XMFLOAT3 Bp4 = { 0.0f, 0.0f, -140.0f };
	XMFLOAT3 Bp5 = { 0.0f, 5.0f, -130.0f };
	XMFLOAT3 Bp6 = { -7.0f, 0.0f, -120.0f };
	XMFLOAT3 Bp7 = { 0.0f, -5.0f, -110.0f };
	XMFLOAT3 Bp8 = { 7.0f, 0.0f, -100.0f };
	XMFLOAT3 Bp9 = { 0.0f, 5.0f, -90.0f };
	XMFLOAT3 Bp10 = { -7.0f, 0.0f, -80.0f };
	XMFLOAT3 Bp11 = { 0.0f, -5.0f, -70.0f };
	XMFLOAT3 Bp12 = { 0.0f, 20.0f, -70.0f };
	XMFLOAT3 Bp13 = { 0.0f, 40.0f, -70.0f };
	XMFLOAT3 Bp14 = { 0.0f, 20.0f, -70.0f };
	XMFLOAT3 Bp15 = { 0.0f, 0.0f, -70.0f };
	XMFLOAT3 Bp16 = { 0.0f, 0.0f, -70.0f };
	XMFLOAT3 Bp17 = { 0.0f, 0.0f, -60.0f };
	XMFLOAT3 Bp18 = { 0.0f, 0.0f, -50.0f };
	XMFLOAT3 Bp19 = { 0.0f, 0.0f, -40.0f };
	XMFLOAT3 Bp20 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp21 = { 15.0f, 0.0f, -10.0f };
	XMFLOAT3 Bp22 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp23 = { -15.0f, 0.0f, 10.0f };
	XMFLOAT3 Bp24 = { 0.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp25 = { 15.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp26 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp27 = { 0.0f, -10.0f, 30.0f };
	XMFLOAT3 Bp28 = { 0.0f, 30.0f, 60.0f };
	XMFLOAT3 Bp29 = { 0.0f, 50.0f, 90.0f };
	XMFLOAT3 Bp30 = { 0.0f, 50.0f, 120.0f };
	XMFLOAT3 Bp31 = { 0.0f, 50.0f, 150.0f };
	XMFLOAT3 Bp32 = { 0.0f, 0.0f, 150.0f };
	XMFLOAT3 Bp33 = { 5.0f, 5.0f, 160.0f };
	XMFLOAT3 Bp34 = { 5.0f, -5.0f, 170.0f };
	XMFLOAT3 Bp35 = { -5.0f, -5.0f, 180.0f };
	XMFLOAT3 Bp36 = { -5.0f, 5.0f, 190.0f };
	XMFLOAT3 Bp37 = { 0.0f, -10.0f, 200.0f };
	XMFLOAT3 Bp38 = { 0.0f, 20.0f, 210.0f };
	XMFLOAT3 Bp39 = { 0.0f, 40.0f, 220.0f };
	XMFLOAT3 Bend = { 0.0f, 60.0f, 230.0f };

	std::vector<XMFLOAT3> bossCheckPoint{ Bstart, Bstart, Bp2, Bp3, Bp4, Bp5, Bp6, Bp7, Bp8, Bp9, Bp10,
		Bp11,Bp12, Bp13, Bp14, Bp15, Bp16, Bp17, Bp18, Bp19, Bp20, Bp21,Bp22, Bp23, Bp24, Bp25, Bp26, Bp27, Bp28,
	Bp29,Bp30,Bp31,Bp32, Bp33, Bp34, Bp35, Bp36, Bp37, Bp38,Bp39,Bend, Bend };

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

	float playerHp;
	float playerHpMax;
	float playerHpRatio;

	// 弾関連
	XMFLOAT3 bulletScale = { 0.3f, 0.3f, 0.3f };

	float bulletSpeed = 1.8f;

	float shotRate = 1.5f;
	bool shotFlag = false;
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

	XMFLOAT3 bossLeg1Velocity = { 0.0f,0.0f,0.0f };

	float bossHp;
	float bossHpMax;
	float bossHpRatio;

	float bossLeg1Hp;
	float bossLeg2Hp;
	float bossLeg3Hp;
	float bossLeg4Hp;

	bool bossBreak = false;
	bool bossLeg1Break = false;
	bool bossLeg2Break = false;
	bool bossLeg3Break = false;
	bool bossLeg4Break = false;

	bool bossFlag = true;
	bool bossLeg1Flag = true;
	bool bossLeg2Flag = true;
	bool bossLeg3Flag = true;
	bool bossLeg4Flag = true;

	// デスフラグ
	bool bossDeathFlag = false;

	enum BOSSPATTERN
	{
		STAY,
		NORMAL,
		BODYUP,
		BODYDOWN,
		BODYRIGHT,
		BODYLEFT,
		RUSH,
		GUARDON,
		GUARDOFF,
		FOURWAYRUSH
	};

	bool rushFlag = false;

	int bossPattern = STAY;

	float moveValue = 0.0f;

	float rushOrder = 0.0f;

	// スペシャル攻撃
	// 弾のサイズ
	XMFLOAT3 specialBulletScale = { 0.0f,0.0f,0.0f };

	// 生成位置
	XMFLOAT3 specialBulletPosition;

	// スピード
	float specialBulletSpeed = 0.0;

#pragma endregion

	XMFLOAT3 centerPosition;
	XMFLOAT3 centerRotation;

	XMFLOAT3 CameraPos;

	XMFLOAT3 cameraFrontPosition;
	XMFLOAT3 cameraRightPosition;
	XMFLOAT3 cameraBackPosition;
	XMFLOAT3 cameraLeftPosition;

	XMFLOAT3 SkydomPos;
	XMFLOAT3 SkydomRot;

#pragma region シーン遷移関連
	enum SCENE
	{
		TITLE,
		STAGESELECT,
		GAME,
		RESULT,
		GAMEOVER
	};

	int scene = TITLE;

	XMFLOAT4 loadingColor;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;
#pragma endregion

#pragma region ステージセレクト関連
	XMFLOAT3 stage1Position;
	XMFLOAT3 stage2Position;
	XMFLOAT3 stage3Position;
#pragma endregion

#pragma region タイトル画面で使う変数
	XMFLOAT3 TitlePlayerPosition;
	XMFLOAT3 TitlePlayerRotation;

	XMFLOAT4 titleStartUIColor;

	int titleScene = WAITING;

	float backTimer = 40.0f;
	float stagingTimer = 60.0f;

	bool stagingFlag = false;

	float moveY = 0.01f;
	float moveX = 0.05f;

	bool addSpeedX = false;
	bool subSpeedX = false;

	bool addSpeedY = false;
	bool subSpeedY = false;

	bool addAngleX = false;
	bool subAngleX = false;

	enum TITLESCENE
	{
		WAITING,
		STAGING,
		MOVESCENE
	};
#pragma endregion

	float testX;
	float testZ;
	float testRadians;
	float testDegrees;

	XMFLOAT3 playerTargetPosition;

	//オーディオのボリューム
	const float Volume_Title = 0.07f;

	// UI関係
	XMFLOAT2 playerHpGageSize;

	XMFLOAT2 bossHpGageSize;

	float playerHpUIHigh = 680.0f;

	float bossHpUIHigh = 30.0f;

	float gameScore = 0.0f;
	float gameScoreMax = 9999999;
	float gameScoreAlpha = 1.0f;
	XMFLOAT2 scoreBasePosition = { 300.0f, 52.0f };

	float scoreMoveVel = 0.0f;
	float scoreMoveAcc = 1.0f;

	XMFLOAT2 resultBN_1Size;
	XMFLOAT2 resultBN_2Size;
	XMFLOAT2 resultBN_3Size;

	float resultMoveVelX_1 = 0.0f;
	float resultMoveAccX_1 = 0.0f;

	float resultMoveVelY_1 = 0.0f;
	float resultMoveAccY_1 = 0.0f;

	float resultMoveVelX_2 = 0.0f;
	float resultMoveAccX_2 = 0.0f;

	float resultMoveVelX_3 = 0.0f;
	float resultMoveAccX_3 = 0.0f;

	float resultMoveVelX_4 = 0.0f;
	float resultMoveAccX_4 = 0.0f;

	float resultMoveVelX_5 = 0.0f;
	float resultMoveAccX_5 = 0.0f;

	float resultMoveVelX_6 = 0.0f;
	float resultMoveAccX_6 = 0.0f;

	float resultMoveVel_7 = 0.0f;

	float resultMoveVel_8 = 0.0f;

	float resultMoveVel_9 = 0.0f;
	float resultMoveVel_10 = 0.0f;
	float resultMoveVel_11 = 0.0f;

	XMFLOAT2 resultParts_1Size;
	XMFLOAT2 resultParts_2Size;
	XMFLOAT2 resultParts_3Size;

	XMFLOAT2 rankSSize;
	XMFLOAT2 rankASize;
	XMFLOAT2 rankBSize;
	XMFLOAT2 rankCSize;
	XMFLOAT2 resultGTXT_15Size;

	XMFLOAT2 missionStar1Size;
	XMFLOAT2 missionStar2Size;
	XMFLOAT2 missionStar3Size;

	XMFLOAT2 scorePosition = { 855.0f, 275.0f };
	XMFLOAT2 noDamageBonusPosition = { 855.0f, 305.0f };
	XMFLOAT2 totalScorePosition = { 800.0f, 385.0f };
	XMFLOAT2 rankPosition = { 800.0f, 455.0f };
	XMFLOAT2 resultRankPosition = { 850.0f,480.0f };

	XMFLOAT4 resultColor;
	XMFLOAT4 resultPartsColor;
	XMFLOAT4 scoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	XMFLOAT4 noDamageBonusColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	XMFLOAT4 totalScoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	XMFLOAT4 rankColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	XMFLOAT4 missionColor = { 0.760f, 0.929f, 1.0f, 0.0f };

	XMFLOAT4 rankSColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 rankAColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 rankBColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 rankCColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	XMFLOAT4 mission1Color;
	XMFLOAT4 mission2Color;
	XMFLOAT4 mission3Color;

	XMFLOAT4 missionStar1Color;
	XMFLOAT4 missionStar2Color;
	XMFLOAT4 missionStar3Color;

	XMFLOAT4 missionStar4Color;
	XMFLOAT4 missionStar5Color;
	XMFLOAT4 missionStar6Color;

	float resultChangeAlpha_1 = 0.0f;
	float resultChangeAlpha_2 = 0.0f;
	float resultChangeAlpha_3 = 0.0f;
	float resultChangeAlpha_4 = 0.0f;
	float resultChangeAlpha_5 = 0.0f;
	float resultChangeAlpha_6 = 0.0f;
	float resultChangeAlpha_7 = 0.0f;
	float resultChangeAlpha_8 = 0.0f;
	float resultChangeAlpha_9 = 0.0f;
	float resultChangeAlpha_10 = 0.0f;
	float resultChangeAlpha_11 = 0.0f;

	float loadingChangeAlpha = 0.0f;

	float damageEffectAlpha = 0.0f;
	float damageEffectAlphaVel = 0.0f;

	float noDamageBonus = 20000.0f;

	float totalScore = 0.0f;

	bool noDamageFlag = true;
	bool targetScoreFlag = false;

	float resultTimer = 5.0f;

	bool canPushKeyFlag = false;

	bool changeResultSizeFlag = false;
};