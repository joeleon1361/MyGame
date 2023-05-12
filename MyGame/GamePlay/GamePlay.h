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
#include "BossParts.h"
#include "TargetBullet.h"

#include "SmallRock.h"
#include "LargeRock.h"
#include "Planet.h"

#include "Spline.h"
#include "Lerp.h"
#include "Easing.h"

#include "GamePad.h"

#include "SceneManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <list>
#include <array>

#include "ScoreManager.h"
#include "VolumeManager.h"
#include "Transform.h"

class CollisionManager;
class Player;
class Bullet;
class PlayerSpecialBullet;
class Boss;
class BossParts;
class BossBullet;
class TargetBullet;
class SmallRock;
class Planet;

// ゲームシーン
class GamePlay : public BaseScene
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

		// ゲーム
		game_parts_1,
		game_gtxt_number1,
		game_gtxt_number2,
		game_gtxt_number3,
		game_gtxt_GO,
		game_gtxt,

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
		result_gtxt_6,
		result_gtxt_7,
		result_gtxt_8,
		result_gtxt_9,
	};

public: // メンバ関数

	// コンストクラタ
	GamePlay();

	// デストラクタ
	~GamePlay();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム処理
	void Update() override;

	// 描画
	void Draw() override;

	// ゲーム初期化
	void GameInitialize();

	// 弾が当たった際のパーティクル生成
	void CreateHitParticles(XMFLOAT3 position);

	// ボス撃破時のパーティクル生成
	void CreateBossParticles(XMFLOAT3 position);

	// プレイヤーのジェットパーティクル生成
	void CreatePlayerJetParticles(XMFLOAT3 position);

	// プレイヤーの飛行機雲パーティクル生成
	void CreatePlayerContrailParticles(XMFLOAT3 position);

	// 弾パーティクル生成
	void CreateBulletParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale);

	// チャージ弾パーティクル生成
	void CreateChargeBulletParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale);

	// ステージオブジェクトパーティクル生成
	void CreateStageBoxParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale);

	// ゲーム画面で使用するのデバックテキスト
	void GameDebugText();

	// カメラモードの切り替え
	void CameraSwitching();

	// プレイヤーの攻撃
	void Attack();

	void chargeAttack();

	void homingAttack();

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

	void ScoreRateTimer();

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

	// 当たり判定
	bool OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB);

	void CreateLargeRockLeft();

	void CreateLargeRockRight();

	void CreateSmallRockLeft();

	void CreateSmallRockRight();

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	DebugText debugText;
	ScoreText scoreText;
	CollisionManager* collisionManager = nullptr;
	ScoreManager* scoreManager = ScoreManager::GetInstance();
	VolumeManager* volumeManager = VolumeManager::GetInstance();
	

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* LoadingBG = nullptr;

	// チャージゲージ
	Sprite* chargeGageBase = nullptr;
	Sprite* chargeGage = nullptr;
	Sprite* chargeGageCover = nullptr;

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

	Sprite* gameGTXT = nullptr;

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

	Sprite* damageEffect = nullptr;

	ParticleManager* bossHitParticle = nullptr;
	ParticleManager* bossBreakParticle = nullptr;
	ParticleManager* playerJetParticle = nullptr;
	ParticleManager* bulletParticle = nullptr;
	ParticleManager* playerContrailParticle = nullptr;
	ParticleManager* stageBoxParticle = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelGround = nullptr;
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelPlayerTurret = nullptr;
	ObjModel* modelBullet = nullptr;
	ObjModel* modelBossLeg = nullptr;
	ObjModel* modelBossCore = nullptr;
	ObjModel* modelBossUpperBody = nullptr;
	ObjModel* modelBossLowerBody = nullptr;
	ObjModel* modelSmallRock = nullptr;
	ObjModel* modelLargeRock = nullptr;

	ObjModel* modelMars = nullptr;
	ObjModel* modelJupiter = nullptr;
	ObjModel* modelCeres = nullptr;
	ObjModel* modelEris = nullptr;
	ObjModel* modelMakemake = nullptr;
	ObjModel* modelHaumea = nullptr;

	FbxModel* testmodel = nullptr;

	ObjObject* objSkydome = nullptr;
	ObjObject* objGround = nullptr;

	// プレイヤー関連
	Player* player = nullptr;
	Player* playerTurret = nullptr;
	ObjObject* objPlayerContrailRight = nullptr;
	ObjObject* objPlayerContrailLeft = nullptr;

	std::list<std::unique_ptr<Bullet>> playerBullets;
	std::list<std::unique_ptr<Bullet>> playerChargeBullets;

	// ボス関連
	Boss* bossBody = nullptr;
	BossParts* bossUpperBody = nullptr;
	BossParts* bossLowerBody = nullptr;
	BossParts* bossLeg1 = nullptr;
	BossParts* bossLeg2 = nullptr;
	BossParts* bossLeg3 = nullptr;
	BossParts* bossLeg4 = nullptr;

	Planet* objMars = nullptr;
	Planet* objJupiter = nullptr;
	Planet* objCeres = nullptr;
	Planet* objEris = nullptr;
	Planet* objMakemake = nullptr;
	Planet* objHaumea = nullptr;

	std::list<std::unique_ptr<TargetBullet>>bossTargetBullets;

	std::list<std::unique_ptr<TargetBullet>>playerTargetBullets;

	ObjObject* objCenter = nullptr;

	ObjObject* objCamera = nullptr;

	FbxObject3d* testobject = nullptr;

	std::list<std::unique_ptr<SmallRock>>smallRocks;
	std::list<std::unique_ptr<LargeRock>>largeRocks;

#pragma region スプライン曲線関連
	// スプライン曲線関連

	// ボス制御点
	XMFLOAT3 Bstart = { 0.0f, 40.0f, 50.0f };
	XMFLOAT3 Bp2 = { 0.0f, 40.0f, 50.0f };
	XMFLOAT3 Bp3 = { 0.0f, 20.0f, 50.0f };
	XMFLOAT3 Bp4 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp5 = { 0.0f, 5.0f, 50.0f };
	XMFLOAT3 Bp6 = { -7.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp7 = { 0.0f, -5.0f, 50.0f };
	XMFLOAT3 Bp8 = { 7.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp9 = { 0.0f, 5.0f, 50.0f };
	XMFLOAT3 Bp10 = { -7.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp11 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp12 = { 0.0f, 30.0f, 50.0f };
	XMFLOAT3 Bp13 = { 0.0f, 60.0f, 50.0f };
	XMFLOAT3 Bp14 = { 0.0f, 60.0f, 50.0f };
	XMFLOAT3 Bp15 = { 0.0f, 30.0f, 50.0f };
	XMFLOAT3 Bp16 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp17 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp18 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp19 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp20 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp21 = { 15.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp22 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp23 = { -15.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp24 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp25 = { 15.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp26 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp27 = { 0.0f, -10.0f, 50.0f };
	XMFLOAT3 Bp28 = { 0.0f, 30.0f, 50.0f };
	XMFLOAT3 Bp29 = { 0.0f, 50.0f, 50.0f };
	XMFLOAT3 Bp30 = { 0.0f, 50.0f, 50.0f };
	XMFLOAT3 Bp31 = { 0.0f, 50.0f, 50.0f };
	XMFLOAT3 Bp32 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp33 = { 5.0f, 5.0f, 50.0f };
	XMFLOAT3 Bp34 = { 5.0f, -5.0f, 50.0f };
	XMFLOAT3 Bp35 = { -5.0f, -5.0f, 50.0f };
	XMFLOAT3 Bp36 = { -5.0f, 5.0f, 50.0f };
	XMFLOAT3 Bp37 = { 0.0f, -10.0f, 50.0f };
	XMFLOAT3 Bp38 = { 0.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp39 = { 10.0f, 0.0f, 50.0f };
	XMFLOAT3 Bp40 = { 20.0f, 0.0f, 50.0f };
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

#pragma region 共通
	// ローディングの暗転色
	XMFLOAT4 loadingColor;

	// シーン遷移時の暗転アルファ値
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

#pragma endregion

	// コアオブジェクトの座標
	XMFLOAT3 centerPosition;
	// コアオブジェクトの角度
	XMFLOAT3 centerRotation;

	// スカイドームの座標
	XMFLOAT3 SkydomPosition;
	// スカイドームの角度
	XMFLOAT3 SkydomRotation;

	// 火星モデルの座標
	XMFLOAT3 marsPosition = {130.0f, 20.0f, 450.0f};

	// 木星モデルの座標
	XMFLOAT3 jupiterPosition = { -150.0f, -30.0f, 250.0f };

	XMFLOAT3 ceresPosition = { -80.0f, -50.0f, 100.0f };

	XMFLOAT3 erisPosition = { 60.0f, -70.0f, -100.0f };

	XMFLOAT3 makemakePosition = { 90.0f, 60.0f, -250.0f };

	XMFLOAT3 haumeaPosition = { -30.0f, 50.0f, -450.0f };

	// コアオブジェクトに加算する値
	float centorVel = 0.1f;

	float scoreRate = 1.0f;
	int scoreRateCount = 0;

	bool helpMenuFlag = false;

	//タイマ
	int32_t rankTimer = 0;
	bool rankSOneTimeFlag = true;
	bool rankAOneTimeFlag = true;
	bool rankBOneTimeFlag = true;
	bool rankCOneTimeFlag = true;

	int32_t scoreRateTimer = 0;
	bool rate1stOneTimeFlag = true;
	bool rate2ndOneTimeFlag = true;
	bool rate3rdOneTimeFlag = true;

	float scoreRateAlpha = 0.0f;

	int32_t creatLargeRockLeftTimer = 0;
	int32_t creatLargeRockRightTimer = 0;

	int32_t creatSmallRockLeftTimer = 0;
	int32_t creatSmallRockRightTimer = 0;

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
	//XMFLOAT3 cameraLocalPosition;

	// 切り替え先のカメラ座標
	XMFLOAT3 nextRotation;
#pragma endregion

#pragma region プレイヤー関連
	// プレイヤーのローカル座標
	XMFLOAT3 playerLocalPosition;
	// プレイヤーの角度
	XMFLOAT3 playerRotation;
	XMFLOAT3 nextPlayerRotation;

	// ゲーム開始時の演出プレイヤーの座標
	XMFLOAT3 startPlayerPosition = { 0.0f,0.0f,-30.0f };

	// プレイヤーのワールド座標
	XMFLOAT3 playerWorldPosition = {};

	XMFLOAT3 playerContrailRightWorldPosition = {};
	XMFLOAT3 playerContrailLeftWorldPosition = {};

	// プレイヤーの弾のサイズ
	XMFLOAT3 bulletScale = { 0.3f, 0.3f, 0.3f };

	// プレイヤーの弾の速度
	float playerBulletSpeed = 1.8f;

	bool playerChargeFlag = false;
	float playerChargeMax = 40.0f;
	float playerChargeNow = 0.0f;
	float playerChargeRatio = 0.0f;
	float chargeBulletSize = 0.0f;

	bool playerHomingFlag = false;
	float playerHomingMax = 180.0f;
	float playerHomingNow = 0.0f;

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

	// ボスの行動パターン
	enum BOSSPATTERN
	{
		STAY,
		NORMAL,
		BODYUP,
		BODYDOWN,
		RUSH
	};

	// ボスのガトリング攻撃フラグ
	bool rushFlag = false;

	// ボスの行動パターン
	int bossPattern = STAY;

	// ボスがどれだけ回転するかの値
	float bossRotateVel = 0.0f;

	// ボスの弾の速度
	float bossBulletSpeed = 0.6f;

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
	// 
	XMFLOAT2 chargeGageSize;

	// プレイヤーのHPバーの座標
	XMFLOAT2 playerHpUIPosition = { 1255.0f, 680.0f };

	// ボスのHPバーの座標
	XMFLOAT2 bossHpUIPosition = { 1255.0f , 30.0f };

	// スコアUIの座標
	XMFLOAT2 scoreUIPosition = { 15.0f, 60.0f };

	// プレイヤーのHPバーの表示座標
	XMFLOAT2 onPlayerHpUIPosition = { 1255.0f, 680.0f };
	// プレイヤーのHPバーの非表示座標
	XMFLOAT2 offPlayerHpUIPosition = { 1255.0f, 780.0f };

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


	// スコアの表示座標
	XMFLOAT2 scoreBasePosition = { 0.0f, 0.0f };
	float scoreMoveVel = 0.0f;
	float scoreMoveAcc = 1.0f;

	// ゲーム開始時のUI出現に加算している値
	float moveUIVel = 0.04f;
#pragma endregion
};