#include "GameScreen.h"
#include "ObjModel.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "FbxLoader.h"
#include "FbxObject.h"
#include "Camera.h"
#include "Player.h"
#include <stdio.h>

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
	Object3d::SetCamera(camera);
	FbxObject3d::SetCamera(camera);


	// デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());

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
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成
	objSkydome = Object3d::Create();
	objGround = Object3d::Create();
	objPlayer = Player::Create(modelPlayer);
	objBullet = Object3d::Create();

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/Sprite/texture.png");

	modelSkydome = Model::CreateFromOBJ("skydome");
	modelGround = Model::CreateFromOBJ("ground");
	modelPlayer = Model::CreateFromOBJ("player2");
	modelBullet = Model::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	objPlayer->SetModel(modelPlayer);
	objBullet->SetModel(modelBullet);

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
	objBullet->SetScale({ 0.5f, 0.5f, 0.5f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, -10 });
	camera->SetUp({ 0, 1, 0 });
}

void GameScene::Update()
{
	XMFLOAT3 playerPosition = objPlayer->GetPosition();
	XMFLOAT3 routeCameraPosition = camera->GetEye();
	XMFLOAT3 cameraTargetPosition = camera->GetTarget();

	MoveCamera();
	// パーティクル生成
	CreateParticles();

	routeCameraPosition.z += 0.05f;
	playerPosition.z += 0.05f;
	cameraTargetPosition.z += 0.05f;

	camera->SetEye(routeCameraPosition);
	camera->SetTarget(cameraTargetPosition);
	objPlayer->SetPosition(playerPosition);

#pragma region 球発射処理
	if (input->PushKey(DIK_SPACE))
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
	}
	#pragma endregion


	camera->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();
	objPlayer->Update();
	objBullet->Update();

	testobject->Update();

#pragma region デバックテキスト
	// プレイヤーの座標を表示
	std::ostringstream PlayerPos;
	PlayerPos << "PlayerPos:("
		<< std::fixed << std::setprecision(2)
		<< playerPosition.x << "," // x
		<< playerPosition.y << "," // y
		<< playerPosition.z << ")"; // z
	debugText.Print(PlayerPos.str(), 50, 50, 1.0f);

	std::ostringstream CameraPos;
	CameraPos << "CameraPos:("
		<< std::fixed << std::setprecision(2)
		<< routeCameraPosition.x << "," // x
		<< routeCameraPosition.y << "," // y
		<< routeCameraPosition.z << ")"; // z
	debugText.Print(CameraPos.str(), 50, 70, 1.0f);

	// 自機操作方法
	debugText.Print("WASD:PlayerMove", 50, 30, 1.0f);
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
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objSkydome->Draw();
	// objGround->Draw();
	objPlayer->Draw();
	if (ShotFlag == 2)
	{
		objBullet->Draw();
	}
	
	// testobject->Draw(cmdList);

	// パーティクルの描画
	//particleMan->Draw(cmdList);

	// ここに3Dオブジェクトの描画処理を追加できる

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
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
