#pragma once

#include "BaseScene.h"

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
#include "StageObject.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"

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
class Stageobject;

// ゲームシーン
class GameScene : public BaseScene
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
	// スプライトのテクスチャ番号
	static enum TextureNumber
	{
		// 共通
		common_dtxt_1,
		common_dtxt_2,

		// タイトル
		title_gtxt_1,
		title_parts_1,

		// ゲーム
		game_parts_1,
		game_gtxt_number1,
		game_gtxt_number2,
		game_gtxt_number3,
		game_gtxt_GO,

		// プレイヤー
		game_player_frame_1,
		game_player_frame_2,
		game_player_gauge,
		game_player_icon,

		// ボス
		game_boss_frame_1,
		game_boss_frame_2,
		game_boss_gauge,

		// スコア
		game_score_frame,
		game_score_gtxt,
		game_score_gtxt_2,
		game_score_parts,

		// エフェクト
		damage_effect_1,
		loading_effect_1,

		// リザルト
		result_bg,
		result_frame_1,
		result_frame_2,
		result_frame_3,
		result_gtxt_1,
		result_gtxt_2,
		result_gtxt_3,
		result_gtxt_4,
		result_gtxt_5,
		result_gtxt_6,
		result_gtxt_7,
		result_gtxt_8,
		result_gtxt_9,
		result_gtxt_12,
		result_gtxt_13,
		result_gtxt_14,
		result_gtxt_15,
		result_parts_1,
		result_parts_2,
		result_parts_3,
		result_parts_4,
		result_parts_5,
		result_parts_6
	};

public: // メンバ関数

	// コンストクラタ
	GameScene();

	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	// タイトル更新
	void TitleUpdate();

	// タイトル描画
	void TitleDraw();

	// タイトル初期化
	void TitleInitialize();

	// ステージセレクト更新
	void StageSelectUpdate();

	// ステージセレクト描画
	void StageSelectDraw();

	// ステージセレクト初期化
	void StageSelectInitialize();

	// ゲーム更新
	void GameUpdate();

	// ゲーム描画
	void GameDraw();

	// ゲーム初期化
	void GameInitialize();

	// リザルト更新
	void ResultUpdate();

	// リザルト描画
	void ResultDraw();

	// リザルト初期化
	void ResultInitialize();

	// ゲームオーバー更新
	void GameOverUpdate();

	// ゲームオーバー描画
	void GameOverDraw();

	// ゲームオーバー初期化
	void GameOverInitialize();

	// 弾が当たった際のパーティクル生成
	void CreateHitParticles(XMFLOAT3 position);

	// ボス撃破時のパーティクル生成
	void CreateBossParticles(XMFLOAT3 position);

	// プレイヤーのジェットパーティクル生成
	void CreatePlayerJetParticles(XMFLOAT3 position);

	// プレイヤーのジェットパーティクル生成
	void CreateTitlePlayerJetParticles(XMFLOAT3 position);

	// プレイヤーの飛行機雲パーティクル生成
	void CreatePlayerContrailParticles(XMFLOAT3 position);

	// プレイヤーの弾パーティクル生成
	void CreatePlayerBulletParticles(XMFLOAT3 position);

	// ボスの弾パーティクル生成
	void CreateBossBulletParticles(XMFLOAT3 position);

	// 全シーン共通のデバックテキスト
	void AllDebugText();

	// タイトル画面で使用するのデバックテキスト
	void TitleDebugText();

	// ゲーム画面で使用するのデバックテキスト
	void GameDebugText();

	// カメラモードの切り替え
	void CameraSwitching();

	// プレイヤーの攻撃
	void Attack();

	// ボスの攻撃
	void BossAttack();

	// ボスの部位1の攻撃
	void BossLeg1Attack();

	// ボスの部位2の攻撃
	void BossLeg2Attack();

	// ボスの部位3の攻撃
	void BossLeg3Attack();

	// ボスの部位4の攻撃
	void BossLeg4Attack();

	// スプライン曲線の計算
	void SplineCount();

	void Lerp1Count();

	void Lerp2Count();

	void Lerp3Count();

	void Lerp4Count();

	void Lerp5Count();

	void RankTimer();

	// テクスチャのロード
	void LoadTextureFunction();

	// Wavデータのロード
	void LoadWavFunction();

	// 得点加算時のUI演出
	void scoreUIMotion();

	// 得点加算時のUI演出の更新
	void scoreUIUpdate();

	// プレイヤーとUIが重なった際のアルファ値計算
	void changeGameUIAlpha();

	// ダメージエフェクトの挙動管理
	void damageEffectUpdate();

	// カメラ切り替え時のUI挙動管理
	void alertUIUpdate();

	// レールオブジェクトの角度計算
	void railTargetCalc();

	// プレイヤーHPの計算
	void playerHpCalc();

	// ボスHPの計算
	void bossHpCalc();

	// 各音量の計算
	void VolumeCalc();

	// 当たり判定
	bool OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB);

	// 雲の生成
	void CreateCloud();

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DebugText debugText;
	ScoreText scoreText;
	CollisionManager* collisionManager = nullptr;

	// ゲームシーン用
	Camera* camera = nullptr;

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
	Sprite* gameGTXT_number1 = nullptr;
	Sprite* gameGTXT_number2 = nullptr;
	Sprite* gameGTXT_number3 = nullptr;
	Sprite* gameGTXT_GO = nullptr;

	Sprite* gameParts_1 = nullptr;
	Sprite* gameParts_2 = nullptr;
	Sprite* gameParts_3 = nullptr;

	// スコアUI
	Sprite* scoreUI = nullptr;
	Sprite* scoreChar = nullptr;
	Sprite* scoreRateChar = nullptr;
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

	ParticleManager* bossHitParticle = nullptr;
	ParticleManager* bossBreakParticle = nullptr;
	ParticleManager* playerJetParticle = nullptr;
	ParticleManager* playerBulletParticle = nullptr;
	ParticleManager* playerContrailParticle = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelGround = nullptr;
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelBullet = nullptr;
	ObjModel* modelBossLeg = nullptr;
	ObjModel* modelCloud_1 = nullptr;
	ObjModel* modelBossCore = nullptr;
	ObjModel* modelBossUpperBody = nullptr;
	ObjModel* modelBossLowerBody = nullptr;

	FbxModel* testmodel = nullptr;

	ObjObject* objSkydome = nullptr;
	ObjObject* objGround = nullptr;
	ObjObject* objTitlePlayer = nullptr;

	ObjObject* objStage1 = nullptr;
	ObjObject* objStage2 = nullptr;
	ObjObject* objStage3 = nullptr;

	ObjObject* objCloud_1 = nullptr;

	// プレイヤー関連
	Player* player = nullptr;
	ObjObject* objPlayerContrailRight = nullptr;
	ObjObject* objPlayerContrailLeft = nullptr;

	std::list<std::unique_ptr<Bullet>> bullets;

	// ボス関連
	Boss* bossBody = nullptr;
	Boss* bossUpperBody = nullptr;
	Boss* bossLowerBody = nullptr;
	Boss* bossLeg1 = nullptr;
	Boss* bossLeg2 = nullptr;
	Boss* bossLeg3 = nullptr;
	Boss* bossLeg4 = nullptr;

	std::list<std::unique_ptr<BossBullet>>bossBullets;

	std::list<std::unique_ptr<BossTargetBullet>>bossTargetBullets;

	ObjObject* objCenter = nullptr;

	ObjObject* objCamera = nullptr;

	FbxObject3d* testobject = nullptr;

	std::list<std::unique_ptr<StageObject>>stageObjects;

#pragma region スプライン曲線関連
	// スプライン曲線関連

	// ボス制御点
	XMFLOAT3 Bstart = { 0.0f, 40.0f, 30.0f };
	XMFLOAT3 Bp2 = { 0.0f, 40.0f, 30.0f };
	XMFLOAT3 Bp3 = { 0.0f, 20.0f, 30.0f };
	XMFLOAT3 Bp4 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp5 = { 0.0f, 5.0f, 30.0f };
	XMFLOAT3 Bp6 = { -7.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp7 = { 0.0f, -5.0f, 30.0f };
	XMFLOAT3 Bp8 = { 7.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp9 = { 0.0f, 5.0f, 30.0f };
	XMFLOAT3 Bp10 = { -7.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp11 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp12 = { 0.0f, 30.0f, 0.0f };
	XMFLOAT3 Bp13 = { 0.0f, 60.0f, -10.0f };
	XMFLOAT3 Bp14 = { 0.0f, 60.0f, -30.0f };
	XMFLOAT3 Bp15 = { 0.0f, 30.0f, -30.0f };
	XMFLOAT3 Bp16 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp17 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp18 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp19 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp20 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp21 = { 15.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp22 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp23 = { -15.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp24 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp25 = { 15.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp26 = { 0.0f, 0.0f, -30.0f };
	XMFLOAT3 Bp27 = { 0.0f, -10.0f, -20.0f };
	XMFLOAT3 Bp28 = { 0.0f, 30.0f, -10.0f };
	XMFLOAT3 Bp29 = { 0.0f, 50.0f, 00.0f };
	XMFLOAT3 Bp30 = { 0.0f, 50.0f, 10.0f };
	XMFLOAT3 Bp31 = { 0.0f, 50.0f, 20.0f };
	XMFLOAT3 Bp32 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp33 = { 5.0f, 5.0f, 30.0f };
	XMFLOAT3 Bp34 = { 5.0f, -5.0f, 30.0f };
	XMFLOAT3 Bp35 = { -5.0f, -5.0f, 30.0f };
	XMFLOAT3 Bp36 = { -5.0f, 5.0f, 30.0f };
	XMFLOAT3 Bp37 = { 0.0f, -10.0f, 30.0f };
	XMFLOAT3 Bp38 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp39 = { 10.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp40 = { 20.0f, 0.0f, 10.0f };
	XMFLOAT3 Bp41 = { 30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp42 = { 30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp43 = { 30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp44 = { 30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp45 = { 30.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp46 = { 20.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp47 = { 10.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp48 = { -10.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp49 = { -20.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp50 = { -30.0f, -10.0f, 0.0f };
	XMFLOAT3 Bp51 = { -30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp52 = { -30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp53 = { -30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp54 = { -30.0f, 0.0f, 0.0f };
	XMFLOAT3 Bp55 = { -30.0f, 0.0f, 10.0f };
	XMFLOAT3 Bp56 = { -20.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp57 = { -10.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp58 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bp59 = { 0.0f, 0.0f, 30.0f };
	XMFLOAT3 Bend = { 0.0f, 0.0f, 30.0f };

	std::vector<XMFLOAT3> bossCheckPoint{ Bstart, Bstart, Bp2, Bp3, Bp4, Bp5, Bp6, Bp7, Bp8, Bp9, Bp10,
		Bp11,Bp12, Bp13, Bp14, Bp15, Bp16, Bp17, Bp18, Bp19, Bp20, Bp21,Bp22, Bp23, Bp24, Bp25, Bp26, Bp27, Bp28,
	Bp29,Bp30,Bp31,Bp32, Bp33, Bp34, Bp35, Bp36, Bp37, Bp38,Bp39,Bp40,Bp41,Bp42,Bp43,Bp44,Bp45,Bp46,Bp47,Bp48,Bp49,
		Bp50,Bp51,Bp52,Bp53,Bp54,Bp55,Bp56,Bp57,Bp58,Bp59,Bend, Bend };

	size_t startIndex = 1;

	bool railCountFlag = true;

	// Soline時間計測に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;
	float elapsedTime;

	float maxTime = 2000.0f;
	float timeRate;

	// Lerp1
	int L1startCount = 0;
	int L1nowCount = 0;
	int L1elapsedCount = 0;
	float L1elapsedTime;

	float L1maxTime = 2000.0f;
	float L1timeRate;

	// Lerp2
	int L2startCount = 0;
	int L2nowCount = 0;
	int L2elapsedCount = 0;
	float L2elapsedTime;

	float L2maxTime = 2000.0f;
	float L2timeRate;

	// Lerp3
	float L3addCount = 0.0f;
	float L3nowCount = 0.1f;

	// Lerp4
	float L4addCount = 0.0f;
	float L4nowCount = 0.0f;

	// Lerp5
	float L5addCount = 0.0f;
	float L5nowCount = 0.0f;

#pragma endregion

#pragma region シーン遷移関連
	// シーンパターン
	enum SCENE
	{
		TITLE,
		STAGESELECT,
		GAME,
		RESULT,
		GAMEOVER
	};

	// シーンパターン
	int scene = TITLE;
#pragma endregion

#pragma region 共通
	// ローディングの暗転色
	XMFLOAT4 loadingColor;

	// シーン遷移時の暗転アルファ値
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

	//オーディオのボリューム
	float masterVolumeNow = 7.0f;
	float masterVolume = 0.0f;

	float bgmVolumeMax = 10.0f;
	float bgmVolumeNow = 10.0f;
	float bgmVolumeRatio = 0.0f;
	float bgmVolume = 0.0f;

	float seVolumeMax = 10.0f;
	float seVolumeNow = 10.0f;
	float seVolumeRatio = 0.0f;
	float seVolume = 0.0f;
#pragma endregion

#pragma region タイトル画面
	// プレイヤー関連
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

#pragma region ステージセレクト関連
	XMFLOAT3 stage1Position;
	XMFLOAT3 stage2Position;
	XMFLOAT3 stage3Position;
#pragma endregion

#pragma region ゲーム画面
	// コアオブジェクトの座標
	XMFLOAT3 centerPosition;
	// コアオブジェクトの角度
	XMFLOAT3 centerRotation;

	// スカイドームの座標
	XMFLOAT3 SkydomPosition;
	// スカイドームの角度
	XMFLOAT3 SkydomRotation;

	// コアオブジェクトに加算する値
	float centorVel = 0.1f;

	float gameScore = 0.0f;
	float gameScoreMax = 9999999.0f;

	float scoreRate = 2.0f;

	bool helpMenuFlag = false;

	//タイマ
	int32_t rankTimer = 0;
	bool rankSOneTimeFlag = true;
	bool rankAOneTimeFlag = true;
	bool rankBOneTimeFlag = true;
	bool rankCOneTimeFlag = true;

	XMFLOAT2 gameGTXT_1Size = {};
	XMFLOAT4 gameGTXT_1Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_2Size = {};
	XMFLOAT4 gameGTXT_2Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_3Size = {};
	XMFLOAT4 gameGTXT_3Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_4Size = {};
	XMFLOAT4 gameGTXT_4Color = { 1.0f, 1.0f, 1.0f, 0.0f };
	
	XMFLOAT2 gameGTXT_Number1Size = {};
	XMFLOAT4 gameGTXT_Number1Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_Number2Size = {};
	XMFLOAT4 gameGTXT_Number2Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_Number3Size = {};
	XMFLOAT4 gameGTXT_Number3Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	XMFLOAT2 gameGTXT_GOSize = {};
	XMFLOAT4 gameGTXT_GOColor = {1.0f, 1.0f, 1.0f, 0.0f};

#pragma region カメラ関連
	// カメラの方向パターン
	enum CAMERADIRECTION
	{
		FRONT,
		RIGHT,
		BACK,
		LEFT
	};

	// カメラのモード
	enum CAMERAMODE
	{
		START,
		FIGHT
	};

	XMFLOAT3 moveCameraPosition_1;
	XMFLOAT3 moveCameraPosition_2;
	XMFLOAT3 moveCameraPosition_3;

	int moveCameraNumber = 0;

	float moveCameraTimer = 2.0f;
	float moveCameraTimerVel = 0.0f;

	int cameraType = START;

	bool breakCameraFlag = false;

	// カメラのワールド座標
	XMFLOAT3 cameraWorldPosition;
	// カメラのローカル座標
	XMFLOAT3 cameraLocalPosition;

	// 切り替え先のカメラ座標
	XMFLOAT3 nextCamera;
#pragma endregion

#pragma region プレイヤー関連
	// プレイヤーのローカル座標
	XMFLOAT3 playerLocalPosition;
	// プレイヤーの角度
	XMFLOAT3 playerRotation;

	// ゲーム開始時の演出プレイヤーの座標
	XMFLOAT3 startPlayerPosition = { 0.0f,0.0f,-30.0f };

	// プレイヤーのワールド座標
	XMFLOAT3 playerWorldPosition = {};

	XMFLOAT3 playerContrailRightWorldPosition = {};
	XMFLOAT3 playerContrailLeftWorldPosition = {};

	// プレイヤーのHP
	float playerHp;
	// プレイヤーの最大HP
	float playerHpMax;
	// プレイヤーの最大HPから現HPを割った値
	float playerHpRatio;

	// プレイヤーの弾のサイズ
	XMFLOAT3 bulletScale = { 0.3f, 0.3f, 0.3f };

	// プレイヤーの弾の速度
	float playerBulletSpeed = 1.8f;
	// ボスの弾の速度
	float bossBulletSpeed = 1.2f;

	// プレイヤーの弾の発射間隔
	float shotRate = 1.5f;
	// プレイヤーの弾の発射フラグ
	bool shotFlag = false;

	// プレイヤーの注視点計算
	XMFLOAT3 playerTargetPosition;
	float testX;
	float testZ;
	float testRadians;
	float testDegrees;
#pragma endregion

#pragma region ボス関連
	// ボスのローカル座標
	XMFLOAT3 bossLocalPosition;
	// ボスの角度
	XMFLOAT3 bossRotation;

	// ボス部位1のローカル座標
	XMFLOAT3 bossLeg1LocalPosition;
	// ボス部位2のローカル座標
	XMFLOAT3 bossLeg2LocalPosition;
	// ボス部位3のローカル座標
	XMFLOAT3 bossLeg3LocalPosition;
	// ボス部位4のローカル座標
	XMFLOAT3 bossLeg4LocalPosition;

	// ボスのワールド座標
	XMFLOAT3 bossWorldPosition;
	// ボス部位1のローカル座標
	XMFLOAT3 bossLeg1WorldPosition;
	// ボス部位2のローカル座標
	XMFLOAT3 bossLeg2WorldPosition;
	// ボス部位3のローカル座標
	XMFLOAT3 bossLeg3WorldPosition;
	// ボス部位4のローカル座標
	XMFLOAT3 bossLeg4WorldPosition;

	// ボスのHP
	float bossHp;
	// ボスの最大HP
	float bossHpMax;
	// ボスの最大HPから現HPを割った値
	float bossHpRatio;

	// ボス部位1のHP
	float bossLeg1Hp;
	// ボス部位2のHP
	float bossLeg2Hp;
	// ボス部位3のHP
	float bossLeg3Hp;
	// ボス部位4のHP
	float bossLeg4Hp;

	// ボスの撃破フラグ
	bool bossBreak = false;
	// ボス部位1の撃破フラグ
	bool bossLeg1Break = false;
	// ボス部位2の撃破フラグ
	bool bossLeg2Break = false;
	// ボス部位3の撃破フラグ
	bool bossLeg3Break = false;
	// ボス部位4の撃破フラグ
	bool bossLeg4Break = false;

	// ボスの生存フラグ
	bool bossFlag = true;
	// ボス部位1の生存フラグ
	bool bossLeg1Flag = true;
	// ボス部位2の生存フラグ
	bool bossLeg2Flag = true;
	// ボス部位3の生存フラグ
	bool bossLeg3Flag = true;
	// ボス部位4の生存フラグ
	bool bossLeg4Flag = true;

	// ボスのデスフラグ
	bool bossDeathFlag = false;
	// ボス部位1のデスフラグ
	bool bossLeg1DeathFlag = false;
	// ボス部位2のデスフラグ
	bool bossLeg2DeathFlag = false;
	// ボス部位3のデスフラグ
	bool bossLeg3DeathFlag = false;
	// ボス部位4のデスフラグ
	bool bossLeg4DeathFlag = false;

	// ボスの行動パターン
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

	// ボスのガトリング攻撃フラグ
	bool rushFlag = false;

	// ボスの行動パターン
	int bossPattern = STAY;

	// ボスがどれだけ回転するかの値
	float bossRotateVel = 0.0f;

	// ボスのガトリング攻撃番号
	int rushOrder = 0;
#pragma endregion

#pragma region UI関係
	// プレイヤーのHPバーのサイズ
	XMFLOAT2 playerHpGageSize;
	// プレイヤーのダメージバーのサイズ
	XMFLOAT2 playerDamageGageSize;

	// ボスのHPバーのサイズ
	XMFLOAT2 bossHpGageSize;
	// ボスのダメージバーのサイズ
	XMFLOAT2 bossDamageGageSize;

	// プレイヤーのHPバーの座標
	XMFLOAT2 playerHpUIPosition = { 1154.0f, 680.0f };

	// ボスのHPバーの座標
	XMFLOAT2 bossHpUIPosition = { 1255.0f , 30.0f };

	// スコアUIの座標
	XMFLOAT2 scoreUIPosition = { 15.0f, 60.0f };

	// プレイヤーのHPバーの表示座標
	XMFLOAT2 onPlayerHpUIPosition = { 1154.0f, 680.0f };
	// プレイヤーのHPバーの非表示座標
	XMFLOAT2 offPlayerHpUIPosition = { 1154.0f, 780.0f };

	// ボスのHPバーの表示座標
	XMFLOAT2 onBossHpUIPosition = { 1255.0f , 30.0f };
	// ボスのHPバーの非表示座標
	XMFLOAT2 offBossHpUIPosition = { 1255.0f , -30.0f };

	// スコアUIの表示座標
	XMFLOAT2 onScoreUIPosition = { 15.0f, 60.0f };
	// スコアUIの非表示座標
	XMFLOAT2 offScoreUIPosition = { 15.0f, -60.0f };

	// 後方向カメラ切り替えUIの色
	XMFLOAT4 gameParts1Color;
	// 右方向カメラ切り替えUIの色
	XMFLOAT4 gameParts2Color;
	// 左方向カメラ切り替えUIの色
	XMFLOAT4 gameParts3Color;

	// ダメージエフェクトのアルファ値
	float damageEffectAlpha = 0.0f;
	// ダメージエフェクトのアルファ値に加算される値
	float damageEffectAlphaVel = 0.0f;

	// スコアUIのアルファ値
	float scoreUIAlpha = 0.0f;
	// スコアUIのアルファ値に加算される値
	float scoreUIAlphaVel = 0.0f;

	// プレイヤーのHPバーのアルファ値
	float playerHpUIAlpha = 0.0f;
	// プレイヤーのHPバーのアルファ値に加算される値
	float playerHpUIAlphaVel = 0.0f;

	// プレイヤーのダメージバーのアルファ値
	float playerDamageUIAlpha = 0.0f;
	// プレイヤーのダメージバーのアルファ値に加算される値
	float playerDamageUIAlphaVel = 0.0f;

	// ボスのHPバーのアルファ値
	float bossHpUIAlpha = 0.0f;
	// ボスのHPバーのアルファ値に加算される値
	float bossHpUIAlphaVel = 0.0f;

	// ボスのダメージバーのアルファ値
	float bossDamageUIAlpha = 0.0f;
	// ボスのダメージバーのアルファ値に加算される値
	float bossDamageUIAlphaVel = 0.0f;

	int backFlashingCount = 0;
	bool backFlashingFlag = false;

	int rightFlashingCount = 0;
	bool rightFlashingFlag = false;

	int leftFlashingCount = 0;
	bool leftFlashingFlag = false;

	// ノーダメージ時に加算加算される値
	float noDamageBonus = 20000.0f;

	// 合計スコアの値
	float totalScore = 0.0f;

	// ノーダメージフラグ
	bool noDamageFlag = true;

	// 目標スコアに達しているかフラグ
	bool targetScoreFlag = false;

	// 全ての部位を破壊したか
	bool allLegBreakFlag = false;

	// リザルト画面に入ってからの時間を計算するタイマー
	float resultTimer = 5.0f;

	// キー入力が可能かフラグ
	bool canPushKeyFlag = false;

	// リザルトテキストのサイズ計算を開始するフラグ
	bool changeResultSizeFlag = false;

	// スコアの表示座標
	XMFLOAT2 scoreBasePosition = { 0.0f, 0.0f };
	float scoreMoveVel = 0.0f;
	float scoreMoveAcc = 1.0f;

	// ゲーム開始時のUI出現に加算している値
	float moveUIVel = 0.04f;
#pragma endregion

#pragma endregion

#pragma region リザルト画面
	// バナー1のサイズ
	XMFLOAT2 resultBN_1Size;
	// バナー2のサイズ
	XMFLOAT2 resultBN_2Size;
	// バナー3のサイズ
	XMFLOAT2 resultBN_3Size;


	XMFLOAT2 resultParts_1Size;
	XMFLOAT2 resultParts_2Size;
	XMFLOAT2 resultParts_3Size;

	// Sランクマークのサイズ
	XMFLOAT2 rankSSize;
	// Aランクマークのサイズ
	XMFLOAT2 rankASize;
	// Bランクマークのサイズ
	XMFLOAT2 rankBSize;
	// Cランクマークのサイズ
	XMFLOAT2 rankCSize;

	XMFLOAT2 resultGTXT_15Size;

	// ミッション達成スター1のサイズ
	XMFLOAT2 missionStar1Size;
	// ミッション達成スター2のサイズ
	XMFLOAT2 missionStar2Size;
	// ミッション達成スター3のサイズ
	XMFLOAT2 missionStar3Size;

	// スコアテキストの表示座標
	XMFLOAT2 scorePosition = { 855.0f, 275.0f };
	// ノーダメージボーナステキストの表示座標
	XMFLOAT2 noDamageBonusPosition = { 855.0f, 305.0f };
	// 合計スコアテキストの表示座標
	XMFLOAT2 totalScorePosition = { 800.0f, 385.0f };
	// クリアランクテキストの表示座標
	XMFLOAT2 rankPosition = { 800.0f, 455.0f };
	// クリアランクマークの表示座標
	XMFLOAT2 resultRankPosition = { 850.0f,480.0f };

	// リザルトテキストの色
	XMFLOAT4 resultColor;
	XMFLOAT4 resultPartsColor;
	// スコアテキストの色
	XMFLOAT4 scoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// ノーダメージテキストの色
	XMFLOAT4 noDamageBonusColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// 合計スコアテキストの色
	XMFLOAT4 totalScoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// クリアランクテキストの色
	XMFLOAT4 rankColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// ミッションテキストの色
	XMFLOAT4 missionColor = { 0.760f, 0.929f, 1.0f, 0.0f };

	// Sランクマークの色
	XMFLOAT4 rankSColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Aランクマークの色
	XMFLOAT4 rankAColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Bランクマークの色
	XMFLOAT4 rankBColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Cランクマークの色
	XMFLOAT4 rankCColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	// ミッション1テキストの色
	XMFLOAT4 mission1Color;
	// ミッション2テキストの色
	XMFLOAT4 mission2Color;
	// ミッション3テキストの色
	XMFLOAT4 mission3Color;

	// ミッション未達成スター1の色
	XMFLOAT4 missionStar1Color;
	// ミッション未達成スター2の色
	XMFLOAT4 missionStar2Color;
	// ミッション未達成スター3の色
	XMFLOAT4 missionStar3Color;

	// ミッション達成スター1の色
	XMFLOAT4 missionStar4Color;
	// ミッション達成スター2の色
	XMFLOAT4 missionStar5Color;
	// ミッション達成スター3の色
	XMFLOAT4 missionStar6Color;

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
#pragma endregion
};