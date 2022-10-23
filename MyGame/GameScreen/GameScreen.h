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

#include "PlayerBullet.h"

class CollisionManager;
class Player;

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

	void CreateDebugText();

	// 移動処理
	void FrontMove();

	void RightMove();


	void BackMove();

	void LeftMove();

	// 移動制限
	void MoveLimitXY();

	void MoveLimitZY();

	void FrontRolling();

	void BackRolling();

	//
	void CameraSwitching();

	void Attack();
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

	ParticleManager* particleMan = nullptr;

	ObjModel* modelSkydome = nullptr;
	ObjModel* modelGround = nullptr;
	ObjModel* modelPlayer = nullptr;
	ObjModel* modelBullet = nullptr;

	FbxModel* testmodel = nullptr;

	ObjObject* objSkydome = nullptr;
	ObjObject* objGround = nullptr;
	Player* objPlayer = nullptr;
	ObjObject* objBullet = nullptr;

	ObjObject* objCenter = nullptr;

	ObjObject* objC = nullptr;

	// ボス関連
	ObjObject* objBossBody = nullptr;
	ObjObject* objBossLeg1 = nullptr;
	ObjObject* objBossLeg2 = nullptr;
	ObjObject* objBossLeg3 = nullptr;
	ObjObject* objBossLeg4 = nullptr;

	FbxObject3d* testobject = nullptr;

	// ボス関連
	int bossHp = 0;
	int bossLegHp1 = 0;
	int bossLegHp2 = 0;
	int bossLegHp3 = 0;
	int bossLegHp4 = 0;

	// カメラ関連
	int cameraMode = 0;

	// スプライン曲線関連
	// プレイヤー制御点
	XMFLOAT3 start = { 10.0f, 0.0f, 0.0f };
	XMFLOAT3 p2 = { -70.0f, 0.0f, -70.0f };
	XMFLOAT3 p3 = { -140.0f, 0.0f, 0.0f };
	XMFLOAT3 end = { -70.0f, 0.0f, 70.0f };
	std::vector<XMFLOAT3> playerCheckPoint{ start, start, p2, p3, end,start, start };

	// ボス制御点
	XMFLOAT3 Bstart = { 10.0f, 0.0f, 20.0f };
	XMFLOAT3 Bp2 = { -70.0f, 0.0f, -50.0f };
	XMFLOAT3 Bp3 = { -150.0f, 0.0f, 20.0f };
	XMFLOAT3 Bend = { -70.0f, 0.0f, 90.0f };
	std::vector<XMFLOAT3> bossCheckPoint{ Bstart, Bstart, Bp2, Bp3, Bend,Bstart, Bstart };

	size_t startIndex = 1;

	// 時間計測に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount = 0;
	float elapsedTime;

	float maxTime = 8000.0f;
	float timeRate;

	XMFLOAT3 playerPosition;
	XMFLOAT3 playerRotation;

	XMFLOAT3 bulletPos;

	XMFLOAT3 CenterPos;

	XMFLOAT3 CameraPos;

	XMFLOAT3 SkydomPos;
	XMFLOAT3 SkydomRot;

	XMFLOAT3 BossPos;
	XMFLOAT3 BossRot;

	const float LimitXZ = 8.5f;
	const float LimitY = 5.0f;

	PlayerBullet* Bullet = nullptr;

	enum SCENE
	{
		TITLE,
		GAME,
		RESULT
	};

	int scene = TITLE;
};

