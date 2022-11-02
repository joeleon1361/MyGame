#include "GameScreen.h"
#include "ObjModel.h"
#include "FbxLoader.h"
#include "FbxObject.h"
#include "Camera.h"
#include "Player.h"
#include "Collider/SphereCollider.h"
#include "Collider/CollisionManager.h"
#include "Collider/Collision.h"

#include <cassert>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <DirectXMath.h>

using namespace DirectX;

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
	safe_delete(spriteBG);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(objPlayer);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelPlayer);
	safe_delete(modelBullet);
	safe_delete(objBullet);

	safe_delete(testmodel);
	safe_delete(testobject);
}

void GameScreen::Initialize(DirectXCommon* dxCommon, Input* input, Sound* audio)
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
	ObjObject::SetCamera(camera);
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
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/Title.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(2, L"Resources/Sprite/Result.png")) {
		assert(0);
		return;
	}

	// 背景スプライト生成
	TitleBG = Sprite::Create(1, { 0.0f,0.0f });
	ResultBG = Sprite::Create(2, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成
	objSkydome = ObjObject::Create();
	objGround = ObjObject::Create();
	objPlayer = Player::Create();
	
	objCenter = ObjObject::Create();

	objBossBody = ObjObject::Create();
	objBossLeg1 = ObjObject::Create();
	objBossLeg2 = ObjObject::Create();
	objBossLeg3 = ObjObject::Create();
	objBossLeg4 = ObjObject::Create();
	objBullet = Bullet::Create(modelBullet);
	objC = ObjObject::Create();

	//objBullet = Bullet::Create(modelBullet);

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objCenter->SetModel(modelBullet);
	objBullet->SetModel(modelBullet);

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

	// objBullet->SetScale({ 0.3f,0.3f,0.3f });

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

void GameScreen::Update()
{
	switch (scene)
	{
	case SCENE::TITLE:
		TitleUpdate();
		break;

	case SCENE::GAME:
		GameUpdate();
		break;

	case SCENE::RESULT:
		ResultUpdata();
		break;
	}
}

void GameScreen::Draw()
{
	switch (scene)
	{
	case SCENE::TITLE:
		TitleDraw();
		break;

	case SCENE::GAME:
		GameDraw();
		break;

	case SCENE::RESULT:
		ResultDraw();
		break;
	}
}

void GameScreen::TitleUpdate()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		GameInitialize();
		scene = GAME;
	}

	// デバックテキスト
	AllDebugText();
}

void GameScreen::TitleDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	TitleBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScreen::TitleInitialize()
{
}

void GameScreen::GameUpdate()
{
	if (startIndex >= 5)
	{
		scene = RESULT;
		startIndex = 1;
	}

	playerPosition = objPlayer->GetPosition();
	playerRotation = objPlayer->GetRotation();


	/*CenterPos = SplinePosition(playerCheckPoint, startIndex, timeRate);

	CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z - 10 };

	SkydomPos = objSkydome->GetPosition();
	SkydomRot = objSkydome->GetRotation();

	BossPos = SplinePosition(bossCheckPoint, startIndex, timeRate);
	BossRot = objBossBody->GetRotation();*/

	BossPos = objBossBody->GetPosition();
	BossRot = objBossBody->GetRotation();

	XMFLOAT3 CRot = objC->GetRotation();

	playerRotation.x += dodgeRollRotation;

	// パーティクル生成
	CreateParticles();

	CRot.x += 2;
	BossRot.y += 2;

	CameraSwitching();

	DodgeRoll();

	if (input->TriggerKey(DIK_SPACE))
	{
		Bullet* newBullet = new Bullet();
		newBullet = Bullet::Create(modelBullet);

		objBullet = newBullet;
	}

	if (objBullet)
	{
		objBullet->Update();
	}

#pragma region スプライン曲線関係
	if (input->PushKey(DIK_R))
	{
		startCount = GetTickCount();
		startIndex = 1;
	}

	nowCount = GetTickCount();

	elapsedCount = nowCount - startCount;
	elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

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

	objPlayer->SetPosition(playerPosition);
	objPlayer->SetRotation(playerRotation);

	camera->SetEye(CameraPos);
	camera->SetTarget(CenterPos);

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

	testobject->Update();

	objCenter->Update();
	objC->Update();
	objPlayer->Update();
	

	objBossBody->Update();
	objBossLeg1->Update();
	objBossLeg2->Update();
	objBossLeg3->Update();
	objBossLeg4->Update();

	//Attack();

	//弾更新
	

	collisionManager->CheckAllCollisions();

	// デバックテキスト
	AllDebugText();
	GameDebugText();
}

void GameScreen::GameDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	// objGround->Draw();
	objPlayer->Draw();

	if (objBullet)
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

	// objCenter->Draw();

	//弾描画
	/*for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw();
	}*/

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScreen::GameInitialize()
{
	// 座標のセット
	objPlayer->SetPosition({ 0,0,0 });
	objPlayer->SetRotation({ 0, 90, 0 });
	objPlayer->SetScale({ 0.5f, 0.5f, 0.5f });

	objCenter->SetPosition({ 0,0,0 });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	objSkydome->SetPosition({ -70,0,0 });

	objBossBody->SetPosition({ 0,0,20 });
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

void GameScreen::ResultUpdata()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		scene = TITLE;
	}

	// デバックテキスト
	AllDebugText();
}

void GameScreen::ResultDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	ResultBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	ObjObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScreen::ResultInitialize()
{
}

void GameScreen::CreateParticles()
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

// デバックテキスト
void GameScreen::AllDebugText()
{
	std::ostringstream Scene;
	Scene << "Scene:("
		<< std::fixed << std::setprecision(2)
		<< scene << ")";
	debugText.Print(Scene.str(), 50, 10, 1.0f);
}

void GameScreen::GameDebugText()
{
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

	/*std::ostringstream startCounter;
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
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);*/

	/*std::ostringstream BossHp;
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
	debugText.Print(BossLegHp4.str(), 50, 330, 1.0f);*/

	std::ostringstream DodgeRollFlag;
	DodgeRollFlag << "DodgeRollFlag:("
		<< std::fixed << std::setprecision(2)
		<< dodgeRollFlag << ")";
	debugText.Print(DodgeRollFlag.str(), 50, 250, 1.0f);

	std::ostringstream DodgeRollVelocity;
	DodgeRollVelocity << "DodgeRollVelocity:("
		<< std::fixed << std::setprecision(2)
		<< dodgeRollVelocity << ")";
	debugText.Print(DodgeRollVelocity.str(), 50, 270, 1.0f);

	std::ostringstream DodgeRollRotation;
	DodgeRollRotation << "DodgeRollRotation:("
		<< std::fixed << std::setprecision(2)
		<< dodgeRollRotation << ")";
	debugText.Print(DodgeRollRotation.str(), 50, 290, 1.0f);

	// 自機操作方法
	debugText.Print("WASD:PlayerMove", 1050, 30, 1.0f);

	debugText.Print("R:CameraReset", 1050, 50, 1.0f);

	debugText.Print("Y:CameraSwitch", 1050, 70, 1.0f);
}
// 前方向移動処理
void GameScreen::FrontMove()
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			playerPosition.y += playerVelocity + dodgeRollVelocity;
		}
		else if (input->PushKey(DIK_S))
		{
			playerPosition.y -= playerVelocity + dodgeRollVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			playerPosition.x += playerVelocity + dodgeRollVelocity;
		}
		else if (input->PushKey(DIK_A))
		{
			playerPosition.x -= playerVelocity + dodgeRollVelocity;
		}
	}
}
// 右方向移動処理
void GameScreen::RightMove()
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			playerPosition.y += playerVelocity;
		}
		else if (input->PushKey(DIK_S))
		{
			playerPosition.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			playerPosition.z -= playerVelocity;
		}
		else if (input->PushKey(DIK_A))
		{
			playerPosition.z += playerVelocity;
		}
	}
}
// 後方向移動処理
void GameScreen::BackMove()
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			playerPosition.y += playerVelocity;
		}
		else if (input->PushKey(DIK_S))
		{
			playerPosition.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			playerPosition.x -= playerVelocity;
		}
		else if (input->PushKey(DIK_A))
		{
			playerPosition.x += playerVelocity;
		}
	}
}
// 左方向移動処理
void GameScreen::LeftMove()
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 移動後の座標を計算
		if (input->PushKey(DIK_W))
		{
			playerPosition.y += playerVelocity;
		}
		else if (input->PushKey(DIK_S))
		{
			playerPosition.y -= playerVelocity;
		}
		if (input->PushKey(DIK_D))
		{
			playerPosition.z += playerVelocity;
		}
		else if (input->PushKey(DIK_A))
		{
			playerPosition.z -= playerVelocity;
		}
	}
}
// XY座標の移動制限
void GameScreen::MoveLimitXY()
{
	// X軸を制限
	playerPosition.x = max(playerPosition.x, -LimitXZ);
	playerPosition.x = min(playerPosition.x, +LimitXZ);

	// Y軸を制限
	playerPosition.y = max(playerPosition.y, -LimitY);
	playerPosition.y = min(playerPosition.y, +LimitY);
}
// XZ座標の移動制限
void GameScreen::MoveLimitZY()
{
	// Z軸を制限
	playerPosition.z = max(playerPosition.z, -LimitXZ);
	playerPosition.z = min(playerPosition.z, +LimitXZ);

	// Y軸を制限
	playerPosition.y = max(playerPosition.y, -LimitY);
	playerPosition.y = min(playerPosition.y, +LimitY);
}
// 前方向時の自機の傾き
void GameScreen::FrontRolling()
{
	// ロール
	if (dodgeRollFlag == 0)
	{
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
// 右方向時の自機の傾き
void GameScreen::RightRolling()
{
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
// 後方向時の自機の傾き
void GameScreen::BackRolling()
{
	// ロール
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
// 左方向時の自機の傾き
void GameScreen::LeftRolling()
{
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

void GameScreen::DodgeRoll()
{
	if (dodgeRollFlag == 0)
	{
		if (input->TriggerKey(DIK_V))
		{
			dodgeRollFlag = 1;
			dodgeRollVelocity = 0.4;
			// dodgeRollRotation = 5.0;
		}
	}
	else if(dodgeRollFlag == 1)
	{
		dodgeRollVelocity -= 0.01f;
		if (dodgeRollVelocity <= 0)
		{
			dodgeRollVelocity = 0.0f;
			dodgeRollFlag = 0;
		}
	}
}
// カメラ方向の切り替え
void GameScreen::CameraSwitching()
{
	if (cameraMode == 0)
	{
		playerPosition.z = 0;

		CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z - 10 };

		// オブジェクト移動
		FrontMove();

		// 移動制限
		MoveLimitXY();

		// ローリング
		FrontRolling();
	}

	else if (cameraMode == 1)
	{
		playerPosition.x = 0;

		CameraPos = { CenterPos.x - 10, CenterPos.y, CenterPos.z };

		// オブジェクト移動
		RightMove();

		// 移動制限
		MoveLimitZY();

		// ローリング
		RightRolling();

	}

	else if (cameraMode == 2)
	{
		playerPosition.z = 0;

		CameraPos = { CenterPos.x, CenterPos.y, CenterPos.z + 10 };

		// オブジェクト移動
		BackMove();

		// 移動制限
		MoveLimitXY();

		// ローリング
		BackRolling();
	}

	else if (cameraMode == 3)
	{
		playerPosition.x = 0;

		CameraPos = { CenterPos.x + 10, CenterPos.y, CenterPos.z };

		// オブジェクト移動
		LeftMove();

		// 移動制限
		MoveLimitZY();

		// ローリング
		LeftRolling();
	}

	else if (cameraMode >= 4)
	{
		cameraMode = 0;
	}

	if (input->TriggerKey(DIK_Y)) { cameraMode += 1; }
}

void GameScreen::Attack()
{
	
}
// スプライン曲線の計算
XMFLOAT3 GameScreen::SplinePosition(const std::vector<XMFLOAT3>& points, size_t startindex, float t)
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