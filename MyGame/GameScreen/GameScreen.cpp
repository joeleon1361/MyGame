#include "GameScreen.h"
#include "ObjModel.h"
#include "FbxLoader.h"
#include "FbxObject.h"
#include "Camera.h"
#include "Player.h"
#include "Collider/SphereCollider.h"
#include "Collider/CollisionManager.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objPlayer);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelPlayer);

	safe_delete(testmodel);
	safe_delete(testobject);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject3d::SetCamera(camera);
	FbxObject3d::SetCamera(camera);

	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

	collisionManager = CollisionManager::GetInstance();

	// グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/background.png")) {
		assert(0);
		return;
	}

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/Sprite/texture.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成
	objSkydome = ObjObject3d::Create();
	objGround = ObjObject3d::Create();
	objPlayer = Player::Create(modelPlayer);
	objBullet = ObjObject3d::Create();
	objCenter = ObjObject3d::Create();

	objBossBody = ObjObject3d::Create();
	objBossLeg1 = ObjObject3d::Create();
	objBossLeg2 = ObjObject3d::Create();
	objBossLeg3 = ObjObject3d::Create();
	objBossLeg4 = ObjObject3d::Create();

	objC = ObjObject3d::Create();

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objBullet->SetModel(modelBullet);
	objCenter->SetModel(modelBullet);

	objBossBody->SetModel(modelBullet);
	objBossLeg1->SetModel(modelBullet);
	objBossLeg2->SetModel(modelBullet);
	objBossLeg3->SetModel(modelBullet);
	objBossLeg4->SetModel(modelBullet);

	objBossBody->SetCollider(new SphereCollider);
	objBossLeg1->SetCollider(new SphereCollider);
	objBossLeg2->SetCollider(new SphereCollider);
	objBossLeg3->SetCollider(new SphereCollider);
	objBossLeg4->SetCollider(new SphereCollider);

	objC->SetModel(modelPlayer);

	objPlayer->SetParent(objCenter);

	objBossLeg1->SetParent(objBossBody);
	objBossLeg2->SetParent(objBossBody);
	objBossLeg3->SetParent(objBossBody);
	objBossLeg4->SetParent(objBossBody);

	objC->SetParent(objPlayer);

	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// 座標のセット
	objPlayer->SetPosition({ 0,0,0 });
	objPlayer->SetRotation({ 0, 90, 0 });
	objPlayer->SetScale({ 0.5f, 0.5f, 0.5f });

	ShotFlag = 0;
	Shot = { 0 , -500.f, 0 };
	objBullet->SetPosition(Shot);
	objBullet->SetScale({ 0.3f, 0.3f, 0.3f });

	objCenter->SetPosition({ 0,0,0 });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	objSkydome->SetPosition({ -70,0,0 });

	objBossBody->SetPosition({ 0,0,0 });
	objBossLeg1->SetPosition({ 2,-2,2 });
	objBossLeg2->SetPosition({ 2,-2,-2 });
	objBossLeg3->SetPosition({ -2,-2,2 });
	objBossLeg4->SetPosition({ -2,-2,-2 });

	objC->SetPosition({ 0,0,5 });
	objC->SetRotation({ 0,0,0 });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	startCount = GetTickCount();
}

void GameScene::Update()
{
	XMFLOAT3 playerPosition = objPlayer->GetPosition();
	XMFLOAT3 playerRotation = objPlayer->GetRotation();

	XMFLOAT3 CenterPos = SplinePosition(playerCheckPoint, startIndex, timeRate);

	XMFLOAT3 CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z - 10 };

	XMFLOAT3 SkydomPos = objSkydome->GetPosition();
	XMFLOAT3 SkydomRot = objSkydome->GetRotation();

	XMFLOAT3 BossPos = SplinePosition(bossCheckPoint, startIndex, timeRate);
	XMFLOAT3 BossRot = objBossBody->GetRotation();

	XMFLOAT3 CRot = objC->GetRotation();

	// MoveCamera();

	// カメラ移動
	/*if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_UP)) { routeCameraPosition.z += 0.06; }
		else if (input->PushKey(DIK_DOWN)) { routeCameraPosition.z -= 0.06;; }
		if (input->PushKey(DIK_RIGHT)) { routeCameraPosition.x += 0.06; }
		else if (input->PushKey(DIK_LEFT)) { routeCameraPosition.x -= 0.06; }
	}*/

	// パーティクル生成
	CreateParticles();

	CRot.x += 2;
	BossRot.y += 2;

#pragma region 四方向カメラ
	if (cameraMode == 0)
	{
		playerPosition.z = 0;

		CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z - 10 };
		
		// オブジェクト移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerPosition.y += 0.3f;
			}
			else if (input->PushKey(DIK_S))
			{
				playerPosition.y -= 0.3f;
			}
			if (input->PushKey(DIK_D))
			{
				playerPosition.x += 0.3f;
			}
			else if (input->PushKey(DIK_A))
			{
				playerPosition.x -= 0.3f;
			}
		}

		// X軸を制限
		playerPosition.x = max(playerPosition.x, -8.5f);
		playerPosition.x = min(playerPosition.x, +8.5f);

		// Y軸を制限
		playerPosition.y = max(playerPosition.y, -5.0f);
		playerPosition.y = min(playerPosition.y, +5.0f);

		// ロール(横)
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_D) && playerRotation.x <= +40.0f)
			{
				playerRotation.x += 5.0f;
			}

			if (input->PushKey(DIK_A) && playerRotation.x >= -40.0f)
			{
				playerRotation.x -= 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerRotation.x != 0.0f)
		{
			if (playerRotation.x >= 0.0f)
			{
				playerRotation.x -= 5.0f;
			}

			if (playerRotation.x <= 0.0f)
			{
				playerRotation.x += 5.0f;
			}
		}

		if (input->PushKey(DIK_SPACE))
		{
			if (ShotFlag == 0)
			{
				ShotFlag = 1;
			}
		}

		/*if (ShotFlag == 1)
		{
			Shot = playerPosition;
			objBullet->SetPosition(Shot);
			ShotFlag = 2;
		}

		if (ShotFlag == 2)
		{
			Shot.z += 2.0f;
			objBullet->SetPosition(Shot);

			if (Shot.z >= 50.0f)
			{
				Shot = { 0, -500.0f, 0 };
				ShotFlag = 0;
			}
		}*/
	}

	else if (cameraMode == 1)
	{
		playerPosition.x = 0;

		CameraPos = { CenterPos.x - 10, CenterPos.y, CenterPos.z };
		
		// オブジェクト移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerPosition.y += 0.3f;
			}
			else if (input->PushKey(DIK_S))
			{
				playerPosition.y -= 0.3f;
			}
			if (input->PushKey(DIK_D))
			{
				playerPosition.z -= 0.3f;
			}
			else if (input->PushKey(DIK_A))
			{
				playerPosition.z += 0.3f;
			}
		}

		// X軸を制限
		playerPosition.z = max(playerPosition.z, -8.5f);
		playerPosition.z = min(playerPosition.z, +8.5f);

		// Y軸を制限
		playerPosition.y = max(playerPosition.y, -5.0f);
		playerPosition.y = min(playerPosition.y, +5.0f);

	}

	else if (cameraMode == 2)
	{
		playerPosition.z = 0;

		CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z + 10 };

		// オブジェクト移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerPosition.y += 0.3f;
			}
			else if (input->PushKey(DIK_S))
			{
				playerPosition.y -= 0.3f;
			}
			if (input->PushKey(DIK_D))
			{
				playerPosition.x -= 0.3f;
			}
			else if (input->PushKey(DIK_A))
			{
				playerPosition.x += 0.3f;
			}
		}

		// X軸を制限
		playerPosition.x = max(playerPosition.x, -8.5f);
		playerPosition.x = min(playerPosition.x, +8.5f);

		// Y軸を制限
		playerPosition.y = max(playerPosition.y, -5.0f);
		playerPosition.y = min(playerPosition.y, +5.0f);

		// ロール(横)
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			if (input->PushKey(DIK_D) && playerRotation.x >= -40.0f)
			{
				playerRotation.x -= 5.0f;
			}

			if (input->PushKey(DIK_A) && playerRotation.x <= +40.0f)
			{
				playerRotation.x += 5.0f;
			}
		}

		// 傾きを戻す
		if (input->PushKey(DIK_A) == 0 && input->PushKey(DIK_D) == 0 && playerRotation.x != 0.0f)
		{
			if (playerRotation.x >= 0.0f)
			{
				playerRotation.x -= 5.0f;
			}

			if (playerRotation.x <= 0.0f)
			{
				playerRotation.x += 5.0f;
			}
		}
	}

	else if (cameraMode == 3)
	{
		playerPosition.x = 0;

		CameraPos = { CenterPos.x + 10, CenterPos.y, CenterPos.z };
		
		// オブジェクト移動
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
		{
			// 移動後の座標を計算
			if (input->PushKey(DIK_W))
			{
				playerPosition.y += 0.3f;
			}
			else if (input->PushKey(DIK_S))
			{
				playerPosition.y -= 0.3f;
			}
			if (input->PushKey(DIK_D))
			{
				playerPosition.z += 0.3f;
			}
			else if (input->PushKey(DIK_A))
			{
				playerPosition.z -= 0.3f;
			}
		}

		// X軸を制限
		playerPosition.z = max(playerPosition.z, -8.5f);
		playerPosition.z = min(playerPosition.z, +8.5f);

		// Y軸を制限
		playerPosition.y = max(playerPosition.y, -5.0f);
		playerPosition.y = min(playerPosition.y, +5.0f);
	}

	else if (cameraMode >= 4)
	{
		cameraMode = 0;
	}

	if (input->TriggerKey(DIK_Y)) { cameraMode += 1; }
#pragma endregion

#pragma region 球発射処理
	/*if (input->PushKey(DIK_SPACE))
	{
		if (ShotFlag == 0)
		{
			ShotFlag = 1;
		}
	}

	if (ShotFlag == 1)
	{
		Shot = playerPosition;
		objBullet->SetPosition(Shot);
		ShotFlag = 2;
	}

	if (ShotFlag == 2)
	{
		Shot.z += 2.0f;
		objBullet->SetPosition(Shot);

		if (Shot.z >= 50.0f)
		{
			Shot = { 0, -500.0f, 0 };
			ShotFlag = 0;
		}
	}*/
#pragma endregion

#pragma region スプライン曲線関係
	if (input->PushKey(DIK_R))
	{
		startCount = GetTickCount();
		startIndex = 1;
	}

	nowCount = GetTickCount();

	elapsedCount = nowCount - startCount;
	float elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

	timeRate = elapsedCount / maxTime;

	if (timeRate >= 1.0f)
	{
		if (startIndex < playerCheckPoint.size() - 3)
		{
			startIndex += 1;
			timeRate -= 1.0f;
			startCount = GetTickCount();
		}
		else
		{
			timeRate = 1.0f;
		}
	}

	if (timeRate >= 1.0f)
	{
		if (startIndex < bossCheckPoint.size() - 3)
		{
			startIndex += 1;
			timeRate -= 1.0f;
			startCount = GetTickCount();
		}
		else
		{
			timeRate = 1.0f;
		}
	}

#pragma endregion

	camera->SetEye(CameraPos);
	camera->SetTarget(CenterPos);

	objPlayer->SetPosition(playerPosition);
	objPlayer->SetRotation(playerRotation);

	objCenter->SetPosition(CenterPos);

	objSkydome->SetPosition(SkydomPos);
	objSkydome->SetRotation(SkydomRot);

	objBossBody->SetPosition(BossPos);
	objBossBody->SetRotation(BossRot);

	objC->SetRotation(CRot);

	// アップデート
	camera->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();
	objPlayer->Update();
	objBullet->Update();
	objCenter->Update();

	testobject->Update();

	objC->Update();

	objBossBody->Update();
	objBossLeg1->Update();
	objBossLeg2->Update();
	objBossLeg3->Update();
	objBossLeg4->Update();

	collisionManager->CheckAllCollisions();

#pragma region デバックテキスト
	// プレイヤーの座標を表示
	std::ostringstream PlayerPos;
	PlayerPos << "PlayerPos:("
		<< std::fixed << std::setprecision(2)
		<< playerPosition.x << "," // x
		<< playerPosition.y << "," // y
		<< playerPosition.z << ") Local"; // z
	debugText.Print(PlayerPos.str(), 50, 30, 1.0f);

	std::ostringstream bossPos;
	bossPos << "BossPos:("
		<< std::fixed << std::setprecision(2)
		<< BossPos.x << "," // x
		<< BossPos.y << "," // y
		<< BossPos.z << ")"; // z
	debugText.Print(bossPos.str(), 50, 50, 1.0f);

	std::ostringstream cameraPos;
	cameraPos << "CameraPos:("
		<< std::fixed << std::setprecision(2)
		<< CameraPos.x << "," // x
		<< CameraPos.y << "," // y
		<< CameraPos.z << ")"; // z
	debugText.Print(cameraPos.str(), 50, 70, 1.0f);

	std::ostringstream startCounter;
	startCounter << "StartCounter:("
		<< std::fixed << std::setprecision(2)
		<< startCount << ")";
	debugText.Print(startCounter.str(), 50, 110, 1.0f);

	std::ostringstream nowCounter;
	nowCounter << "NowCounter:("
		<< std::fixed << std::setprecision(2)
		<< nowCount << ")";
	debugText.Print(nowCounter.str(), 50, 130, 1.0f);

	std::ostringstream elapsedCounter;
	elapsedCounter << "ElapsedCounter:("
		<< std::fixed << std::setprecision(2)
		<< elapsedCount << ")";
	debugText.Print(elapsedCounter.str(), 50, 150, 1.0f);

	std::ostringstream elapsedTimer;
	elapsedTimer << "ElapsedTimer:("
		<< std::fixed << std::setprecision(2)
		<< elapsedTime << "[s])";
	debugText.Print(elapsedTimer.str(), 50, 170, 1.0f);

	std::ostringstream TimeRate;
	TimeRate << "TimeRate:("
		<< std::fixed << std::setprecision(2)
		<< timeRate << "[%])";
	debugText.Print(TimeRate.str(), 50, 190, 1.0f);

	std::ostringstream StartIndex;
	StartIndex << "StartIndex:("
		<< std::fixed << std::setprecision(2)
		<< startIndex << ")";
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);

	std::ostringstream BossHp;
	BossHp << "BossHp:("
		<< std::fixed << std::setprecision(2)
		<< bossHp << ")";
	debugText.Print(BossHp.str(), 50, 250, 1.0f);

	std::ostringstream BossLegHp1;
	BossLegHp1 << "BossLegHp1:("
		<< std::fixed << std::setprecision(2)
		<< bossLegHp1 << ")";
	debugText.Print(BossLegHp1.str(), 50, 270, 1.0f);

	std::ostringstream BossLegHp2;
	BossLegHp2 << "BossLegHp2:("
		<< std::fixed << std::setprecision(2)
		<< bossLegHp2 << ")";
	debugText.Print(BossLegHp2.str(), 50, 290, 1.0f);

	std::ostringstream BossLegHp3;
	BossLegHp3 << "BossLegHp3:("
		<< std::fixed << std::setprecision(2)
		<< bossLegHp3 << ")";
	debugText.Print(BossLegHp3.str(), 50, 310, 1.0f);

	std::ostringstream BossLegHp4;
	BossLegHp4 << "BossLegHp4:("
		<< std::fixed << std::setprecision(2)
		<< bossLegHp4 << ")";
	debugText.Print(BossLegHp4.str(), 50, 330, 1.0f);

	// 自機操作方法
	debugText.Print("WASD:PlayerMove", 50, 10, 1.0f);
#pragma endregion
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	// ここに背景スプライトの描画処理を追加できる

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	// objGround->Draw();
	objPlayer->Draw();
	if (ShotFlag == 2)
	{
		objBullet->Draw();
	}

	// objC->Draw();

	objBossBody->Draw();
	objBossLeg1->Draw();
	objBossLeg2->Draw();
	objBossLeg3->Draw();
	objBossLeg4->Draw();

	// testobject->Draw(cmdList);

	objCenter->Draw();

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// ここに3Dオブジェクトの描画処理を追加できる

	// 3Dオブジェクト描画後処理
	ObjObject3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// ここに前景スプライトの描画処理を追加できる

	// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::MoveCamera()
{
	// カメラ移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		if (input->PushKey(DIK_UP)) { camera->MoveVector({ 0.0f,+0.06f,0.0f }); }
		else if (input->PushKey(DIK_DOWN)) { camera->MoveVector({ 0.0f,-0.06f,0.0f }); }
		if (input->PushKey(DIK_RIGHT)) { camera->MoveVector({ +0.06f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_LEFT)) { camera->MoveVector({ -0.06f,0.0f,0.0f }); }
	}

	// カメラ移動
	if (input->PushKey(DIK_I) || input->PushKey(DIK_K))
	{
		if (input->PushKey(DIK_I)) { camera->MoveVector({ 0.0f,0.0f,+0.06f }); }
		else if (input->PushKey(DIK_K)) { camera->MoveVector({ 0.0f,0.0f,-0.06f }); }
	}
}

void GameScene::CreateParticles()
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
	}
}

XMFLOAT3 GameScene::SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex > n) return points[n];
	if (startIndex < 1) return points[1];

	XMFLOAT3 p0 = points[startindex - 1];
	XMFLOAT3 p1 = points[startindex];
	XMFLOAT3 p2 = points[startindex + 1];
	XMFLOAT3 p3 = points[startindex + 2];

	/*XMFLOAT3 position = { 0.5 * (2 * p1 + (-p0 + p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
		(-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t) };*/

	XMFLOAT3 A, B, C, D, E, F, G, H, I, J;
	A = XMFLOAT3(2 * p1.x, 2 * p1.y, 2 * p1.z);
	B = XMFLOAT3(-p0.x, -p0.y, -p0.z);
	C = XMFLOAT3(p2.x, p2.y, p2.z);
	D = XMFLOAT3(2 * p0.x, 2 * p0.y, 2 * p0.z);
	E = XMFLOAT3(-5 * p1.x, -5 * p1.y, -5 * p1.z);
	F = XMFLOAT3(4 * p2.x, 4 * p2.y, 4 * p2.z);
	G = XMFLOAT3(-p3.x, -p3.y, -p3.z);
	H = XMFLOAT3(3 * p1.x, 3 * p1.y, 3 * p1.z);
	I = XMFLOAT3(-3 * p2.x, -3 * p2.y, -3 * p2.z);
	J = XMFLOAT3(p3.x, p3.y, p3.z);

	/*XMFLOAT3 position = { 0.5 * (A + (B + C) * t +
		(D + E + F + G) * t * t +
		(B + H + I + J) * t * t * t) };*/

	XMFLOAT3 K, L, M;
	K = { A.x + (B.x + C.x) * t, A.y + (B.y + C.y) * t, A.z + (B.z + C.z) * t };
	L = { (D.x + E.x + F.x + G.x) * t * t, (D.y + E.y + F.y + G.y) * t * t, (D.z + E.z + F.z + G.z) * t * t };
	M = { (B.x + H.x + I.x + J.x) * t * t * t, (B.y + H.y + I.y + J.y) * t * t * t, (B.z + H.z + I.z + J.z) * t * t * t };

	XMFLOAT3 N;
	N = { K.x + L.x + M.x, K.y + L.y + M.y ,K.z + L.z + M.z };

	XMFLOAT3 position = { N.x * 0.5f, N.y * 0.5f, N.z * 0.5f };

	return position;
}