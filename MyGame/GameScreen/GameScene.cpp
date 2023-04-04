#include "GameScene.h"

extern int cameraMode = 0;
extern bool playerUpdateFlag = false;

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	//コントローラー初期化
	InitInput();

	// サウンド初期化
	Sound::GetInstance()->Initialize();

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
	if (!Sprite::LoadTexture(common_dtxt_1, L"Resources/Sprite/Common/common_dtxt_1.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Initialize(common_dtxt_1);

	// スコアテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(common_dtxt_2, L"Resources/Sprite/Common/common_dtxt_2.png")) {
		assert(0);
		return;
	}
	// スコアテキスト初期化
	scoreText.Initialize(common_dtxt_2);

	//音声の読み込み
	LoadWavFunction();

	// テクスチャ読み込みw
	LoadTextureFunction();

	// スプライト生成
	// タイトル
	TitleLogo = Sprite::Create(TextureNumber::title_gtxt_1, { 100.0f,100.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	TitleStartUI = Sprite::Create(TextureNumber::title_parts_1, { 310.0f,630.0f });

	// ゲーム
	gameParts_1 = Sprite::Create(TextureNumber::game_parts_1, { 640.0f,600.0f });
	gameParts_2 = Sprite::Create(TextureNumber::game_parts_1, { 1160.0f,360.0f });
	gameParts_3 = Sprite::Create(TextureNumber::game_parts_1, { 120.0f,360.0f });

	gameGTXT_number1 = Sprite::Create(TextureNumber::game_gtxt_number1, { 0.0f,0.0f });
	gameGTXT_number2 = Sprite::Create(TextureNumber::game_gtxt_number2, { 0.0f,0.0f });
	gameGTXT_number3 = Sprite::Create(TextureNumber::game_gtxt_number3, { 0.0f,0.0f });
	gameGTXT_GO = Sprite::Create(TextureNumber::game_gtxt_GO, { 0.0f,0.0f });

	// プレイヤー
	playerHpUI = Sprite::Create(TextureNumber::game_player_frame_1, { playerHpUIPosition.x + 10.0f, playerHpUIPosition.y });
	playerHpGage = Sprite::Create(TextureNumber::game_player_gauge, playerHpUIPosition);
	playerDamageGage = Sprite::Create(TextureNumber::game_player_gauge, playerHpUIPosition);
	playerHpUICover = Sprite::Create(TextureNumber::game_player_frame_2, { playerHpUIPosition.x + 10.0f, playerHpUIPosition.y });

	// ボス
	bossHpUI = Sprite::Create(TextureNumber::game_boss_frame_1, { bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });
	bossHpGage = Sprite::Create(TextureNumber::game_boss_gauge, bossHpUIPosition);
	bossDamageGage = Sprite::Create(TextureNumber::game_boss_gauge, bossHpUIPosition);
	bossHpUICover = Sprite::Create(TextureNumber::game_boss_frame_2, { bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });

	// スコア
	scoreUI = Sprite::Create(TextureNumber::game_score_frame, scoreUIPosition);
	scoreChar = Sprite::Create(TextureNumber::game_score_gtxt, { 190.0f, scoreUIPosition.y - 32.0f });
	scoreRateChar = Sprite::Create(TextureNumber::game_score_gtxt_2, { 190.0f, scoreUIPosition.y - 32.0f });
	scoreNull_1 = Sprite::Create(TextureNumber::game_score_parts, { 403.5f, scoreUIPosition.y - 33.0f });
	scoreNull_2 = Sprite::Create(TextureNumber::game_score_parts, { 378.0f, scoreUIPosition.y - 33.0f });
	scoreNull_3 = Sprite::Create(TextureNumber::game_score_parts, { 352.5f, scoreUIPosition.y - 33.0f });
	scoreNull_4 = Sprite::Create(TextureNumber::game_score_parts, { 326.5f, scoreUIPosition.y - 33.0f });
	scoreNull_5 = Sprite::Create(TextureNumber::game_score_parts, { 300.5f, scoreUIPosition.y - 33.0f });
	scoreNull_6 = Sprite::Create(TextureNumber::game_score_parts, { 275.0f, scoreUIPosition.y - 33.0f });

	// S
	gameGTXT_1 = Sprite::Create(result_gtxt_6, { 66.0f,scoreUIPosition.y + 2.0f });
	// A
	gameGTXT_2 = Sprite::Create(result_gtxt_7, { 66.0f,scoreUIPosition.y + 2.0f });
	// B
	gameGTXT_3 = Sprite::Create(result_gtxt_8, { 66.0f,scoreUIPosition.y + 2.0f });
	// C
	gameGTXT_4 = Sprite::Create(result_gtxt_9, { 66.0f,scoreUIPosition.y + 2.0f });

	// エフェクト
	LoadingBG = Sprite::Create(TextureNumber::loading_effect_1, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f });
	damageEffect = Sprite::Create(TextureNumber::damage_effect_1, { 0.0f, 0.0 });

	// リザルト
	ResultBG = Sprite::Create(TextureNumber::result_bg, { 0.0f,0.0f });
	ResultBN_1 = Sprite::Create(TextureNumber::result_frame_1, { 640.0f,360.0f });
	ResultBN_2 = Sprite::Create(TextureNumber::result_frame_2, { 640.0f,320.0f });
	ResultBN_3 = Sprite::Create(TextureNumber::result_frame_3, { 640.0f,520.0f });

	resultGTXT_1 = Sprite::Create(TextureNumber::result_gtxt_1, { 330.0f,270.0f });
	resultGTXT_2 = Sprite::Create(TextureNumber::result_gtxt_2, { 330.0f,300.0f });
	resultGTXT_3 = Sprite::Create(TextureNumber::result_gtxt_3, { 330.0f,340.0f });
	resultGTXT_4 = Sprite::Create(TextureNumber::result_gtxt_4, { 330.0f,455.0f });
	resultGTXT_5 = Sprite::Create(TextureNumber::result_gtxt_5, { 600.0f,455.0f });
	resultGTXT_6 = Sprite::Create(TextureNumber::result_gtxt_6, { 500.0f,550.0f });
	resultGTXT_7 = Sprite::Create(TextureNumber::result_gtxt_7, { 500.0f,550.0f });
	resultGTXT_8 = Sprite::Create(TextureNumber::result_gtxt_8, { 500.0f,550.0f });
	resultGTXT_9 = Sprite::Create(TextureNumber::result_gtxt_9, { 500.0f,550.0f });
	resultGTXT_12 = Sprite::Create(TextureNumber::result_gtxt_12, { 600.0f,510.0f });
	resultGTXT_13 = Sprite::Create(TextureNumber::result_gtxt_13, { 600.0f,545.0f });
	resultGTXT_14 = Sprite::Create(TextureNumber::result_gtxt_14, { 600.0f,580.0f });
	resultGTXT_15 = Sprite::Create(TextureNumber::result_gtxt_15, { 640.0f,120.0f });

	resultParts_1 = Sprite::Create(TextureNumber::result_parts_1, { 640.0f,320.0f });
	resultParts_2 = Sprite::Create(TextureNumber::result_parts_2, { 435.0f,480.0f });
	resultParts_3 = Sprite::Create(TextureNumber::result_parts_3, { 775.0f,480.0f });
	resultParts_4 = Sprite::Create(TextureNumber::result_parts_4, { 930.0f,510.0f });
	resultParts_5 = Sprite::Create(TextureNumber::result_parts_4, { 930.0f,545.0f });
	resultParts_6 = Sprite::Create(TextureNumber::result_parts_4, { 930.0f,580.0f });
	resultParts_7 = Sprite::Create(TextureNumber::result_parts_5, { 930.0f,510.0f });
	resultParts_8 = Sprite::Create(TextureNumber::result_parts_5, { 930.0f,545.0f });
	resultParts_9 = Sprite::Create(TextureNumber::result_parts_5, { 930.0f,580.0f });
	resultParts_10 = Sprite::Create(TextureNumber::result_parts_6, { 640.0f,120.0f });

	// パーティクルマネージャー
	bossHitParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect6.png");
	bossBreakParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	playerJetParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	playerContrailParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	playerBulletParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	// 3Dオブジェクト生成
	objSkydome = ObjObject::Create();
	objGround = ObjObject::Create();

	objStage1 = ObjObject::Create();
	objStage2 = ObjObject::Create();
	objStage3 = ObjObject::Create();

	objCloud_1 = ObjObject::Create();

	player = Player::Create();
	objPlayerContrailRight = ObjObject::Create();
	objPlayerContrailLeft = ObjObject::Create();

	objCenter = ObjObject::Create();

	objCamera = ObjObject::Create();

	bossBody = Boss::Create();
	bossUpperBody = Boss::Create();
	bossLowerBody = Boss::Create();
	bossLeg1 = Boss::Create();
	bossLeg2 = Boss::Create();
	bossLeg3 = Boss::Create();
	bossLeg4 = Boss::Create();

	objTitlePlayer = ObjObject::Create();

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");
	modelBossLeg = ObjModel::CreateFromOBJ("bossLeg");
	modelCloud_1 = ObjModel::CreateFromOBJ("test");
	modelBossCore = ObjModel::CreateFromOBJ("bossCore");
	modelBossUpperBody = ObjModel::CreateFromOBJ("bossUpperBody");
	modelBossLowerBody = ObjModel::CreateFromOBJ("bossLowerBody");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	player->SetModel(modelPlayer);
	objPlayerContrailRight->SetModel(modelBullet);
	objPlayerContrailLeft->SetModel(modelBullet);

	objCenter->SetModel(modelBullet);
	objCamera->SetModel(modelBullet);

	objStage1->SetModel(modelBullet);
	objStage2->SetModel(modelBullet);
	objStage3->SetModel(modelBullet);

	objCloud_1->SetModel(modelCloud_1);

	bossBody->SetModel(modelBossCore);
	bossUpperBody->SetModel(modelBossUpperBody);
	bossLowerBody->SetModel(modelBossLowerBody);
	bossLeg1->SetModel(modelBossLeg);
	bossLeg2->SetModel(modelBossLeg);
	bossLeg3->SetModel(modelBossLeg);
	bossLeg4->SetModel(modelBossLeg);

	objTitlePlayer->SetModel(modelPlayer);

	// 親子関係を結ぶ
	player->SetParent(objCenter);
	objPlayerContrailRight->SetParent(player);
	objPlayerContrailLeft->SetParent(player);

	objCamera->SetParent(objCenter);

	bossBody->SetParent(objCenter);
	bossUpperBody->SetParent(bossBody);
	bossLowerBody->SetParent(bossBody);

	bossLeg1->SetParent(bossLowerBody);
	bossLeg2->SetParent(bossLowerBody);
	bossLeg3->SetParent(bossLowerBody);
	bossLeg4->SetParent(bossLowerBody);

	objCloud_1->SetBillboard(true);

	// FBXモデルのロード
	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// 各音量初期化
	VolumeCalc();

	// タイトル画面の初期化
	TitleInitialize();
}

void GameScene::Finalize()
{
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(player);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelPlayer);
	safe_delete(modelBullet);

	safe_delete(testmodel);
	safe_delete(testobject);

	Sound::GetInstance()->Finalize();
}

void GameScene::Update()
{
	// 音量の計算
	VolumeCalc();

	switch (scene)
	{
	case SCENE::TITLE:
		TitleUpdate();
		break;

	case SCENE::STAGESELECT:
		StageSelectUpdate();
		break;

	case SCENE::GAME:
		GameUpdate();
		break;

	case SCENE::RESULT:
		ResultUpdate();
		break;

	case SCENE::GAMEOVER:
		GameOverUpdate();
		break;
	}
}

void GameScene::Draw()
{
	switch (scene)
	{
	case SCENE::TITLE:
		TitleDraw();
		break;

	case SCENE::STAGESELECT:
		StageSelectDraw();
		break;

	case SCENE::GAME:
		GameDraw();
		break;

	case SCENE::RESULT:
		ResultDraw();
		break;

	case SCENE::GAMEOVER:
		GameOverDraw();
		break;
	}
}

void GameScene::TitleUpdate()
{
#pragma region 情報の取得
	SkydomRotation = objSkydome->GetRotation();
	TitlePlayerPosition = objTitlePlayer->GetPosition();
	TitlePlayerRotation = objTitlePlayer->GetRotation();

	SkydomRotation.y += 0.4f;

	loadingColor = LoadingBG->GetColor();

	titleStartUIColor = TitleStartUI->GetColor();
#pragma endregion

#pragma region プレイヤーモデルの制御
	TitlePlayerPosition.x += moveX;
	TitlePlayerPosition.y += moveY;

	if (TitlePlayerPosition.y >= -1.0f)
	{
		subSpeedY = true;
	}

	if (TitlePlayerPosition.y <= -3.0f)
	{
		addSpeedY = true;
	}

	if (subSpeedY == true)
	{
		moveY -= 0.001f;

		if (moveY <= -0.01f)
		{
			subSpeedY = false;
		}

	}
	if (addSpeedY == true)
	{
		moveY += 0.001f;

		if (moveY >= 0.01f)
		{
			addSpeedY = false;
		}

	}
#pragma endregion

	CreateTitlePlayerJetParticles(TitlePlayerPosition);

	switch (titleScene)
	{
	case TITLESCENE::WAITING:

		if (TitlePlayerPosition.x >= 0.0f)
		{
			subSpeedX = true;
		}

		if (TitlePlayerPosition.x <= -4.0f)
		{
			addSpeedX = true;
		}

		if (subSpeedX == true)
		{
			moveX -= 0.001f;

			if (moveX <= -0.05f)
			{
				subSpeedX = false;
			}

		}
		if (addSpeedX == true)
		{
			moveX += 0.001f;

			if (moveX >= 0.05f)
			{
				addSpeedX = false;
			}

		}

		// UIの点滅
		titleStartUIColor.w -= 0.01f;

		if (titleStartUIColor.w <= 0.3f)
		{
			titleStartUIColor.w = 1.0f;
		}

		// 特定のキーを押してシーン遷移開始
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerKey(DIK_W) || Input::GetInstance()->TriggerKey(DIK_A) || Input::GetInstance()->TriggerKey(DIK_S) || Input::GetInstance()->TriggerKey(DIK_D))
		{
			Sound::GetInstance()->PlayWav("SE/Title/title_start.wav", seVolume);
			titleStartUIColor.w = 1.0f;
			titleScene = STAGING;
		}

		break;

	case TITLESCENE::STAGING:
		stagingTimer--;
		titleStartUIColor.w -= 0.02f;

		if (moveX >= 0.0f)
		{
			moveX -= 0.001f;
		}
		if (moveX <= 0.0f)
		{
			moveX += 0.001f;
		}

		if (stagingTimer <= 0.0f)
		{
			titleScene = MOVESCENE;
			stagingFlag = true;
		}

		break;

	case TITLESCENE::MOVESCENE:

		if (stagingFlag == true)
		{
			backTimer--;
			moveX -= 0.005f;
			if (backTimer <= 0.0f)
			{
				stagingFlag = false;
			}
		}

		if (stagingFlag == false)
		{
			if (TitlePlayerPosition.x <= 50.0f)
			{
				moveX += 0.05f;
			}
			changeColorTimer--;
		}

		if (changeColorTimer <= 0)
		{
			changeColorFlag = true;
			if (changeColorFlag == true)
			{
				loadingColor.w += 0.05f;
				changeSceneFlag = true;
			}
		}

		if (changeSceneFlag == true)
		{
			changeSceneTimer--;
		}

		if (changeSceneTimer <= 0)
		{
			Sound::GetInstance()->StopWav("BGM/Title/title_bgm.wav");
			GameInitialize();
			scene = GAME;
		}

		break;
	}

#pragma region 情報のセット
	objSkydome->SetRotation(SkydomRotation);
	objTitlePlayer->SetPosition(TitlePlayerPosition);
	objTitlePlayer->SetRotation(TitlePlayerRotation);

	LoadingBG->SetColor(loadingColor);

	TitleStartUI->SetColor(titleStartUIColor);
#pragma endregion

#pragma region 更新処理
	camera->Update();
	objSkydome->Update();
	objTitlePlayer->Update();
	playerJetParticle->Update();
#pragma endregion

	// デバックテキスト
	AllDebugText();
	TitleDebugText();
}

void GameScene::TitleDraw()
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
	objTitlePlayer->Draw();


	// パーティクルの描画
	bossHitParticle->Draw(cmdList);
	playerJetParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	TitleLogo->Draw();

	TitleStartUI->Draw();

	LoadingBG->Draw();

	// デバッグテキストの描画
	// debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::TitleInitialize()
{
	objTitlePlayer->SetPosition({ -4.0f,-2.0f,0 });
	objTitlePlayer->SetRotation({ 0, 180, 0 });
	objTitlePlayer->SetScale({ 0.8f, 0.8f, 0.8f });

	objSkydome->SetPosition({ 0.0f, 0.0f, 70.0f });
	objSkydome->SetScale({ 1.0f,1.0f,1.0f });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	// LoadingBG->SetColor({ 1, 1, 1, 0 });
	loadingColor.w = 0;

	TitleStartUI->SetColor({ 1, 1, 1, 1 });

	backTimer = 40.0f;
	stagingTimer = 60.0f;

	moveX = 0.05f;

	stagingFlag = false;
	moveY = 0.01f;

	addSpeedX = false;
	subSpeedX = false;

	addSpeedY = false;
	subSpeedY = false;

	titleScene = WAITING;

	changeColorFlag = false;
	changeColorTimer = 30.0f;

	changeSceneFlag = false;
	changeSceneTimer = 100.0f;

	Sound::GetInstance()->PlayWav("BGM/Title/title_bgm.wav", bgmVolume, true);
}

void GameScene::StageSelectUpdate()
{
	//SkydomRotation = objSkydome->GetRotation();

	stage1Position = objStage1->GetPosition();
	stage2Position = objStage2->GetPosition();
	stage3Position = objStage3->GetPosition();

	loadingColor = LoadingBG->GetColor();

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		changeColorFlag = true;
	}

	if (changeColorFlag == true)
	{
		loadingColor.w += 0.05f;
		changeSceneFlag = true;
	}

	if (changeSceneFlag == true)
	{
		changeSceneTimer--;
	}

	if (changeSceneTimer <= 0)
	{
		GameInitialize();
		scene = GAME;
	}

	objStage1->SetPosition(stage1Position);
	objStage2->SetPosition(stage2Position);
	objStage3->SetPosition(stage3Position);

	LoadingBG->SetColor(loadingColor);

	camera->Update();
	objSkydome->Update();

	objStage1->Update();
	objStage2->Update();
	objStage3->Update();
}

void GameScene::StageSelectDraw()
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

	objStage1->Draw();
	objStage2->Draw();
	objStage3->Draw();

	// パーティクルの描画
	bossHitParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	//StageSelectBG->Draw();

	LoadingBG->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::StageSelectInitialize()
{
	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });

	objStage1->SetPosition({ 40.0f, 0.0f, -10.0f });
	objStage1->SetScale({ 5.0f, 5.0f, 5.0f });

	objStage2->SetPosition({ -30.0f, 0.0f, 20.0f });
	objStage2->SetScale({ 5.0f, 5.0f, 5.0f });

	objStage3->SetPosition({ 20.0f, 0.0f, 50.0f });
	objStage3->SetScale({ 5.0f, 5.0f, 5.0f });

	camera->SetTarget({ 0, 0, 20 });
	camera->SetEye({ 0, 50, 100 });
	camera->SetUp({ 0, 1, 0 });

	LoadingBG->SetColor({ 1, 1, 1, 0 });

	changeColorFlag = false;
	changeColorTimer = 30.0f;

	changeSceneFlag = false;
	changeSceneTimer = 100.0f;
}

void GameScene::GameUpdate()
{
	// ルートを完走したら遷移


	// ボスが撃破されたら遷移
	if (bossFlag == false)
	{
		changeColorFlag = true;
	}

	// プレイヤーが撃破されたら遷移
	if (playerHp <= 0.0f)
	{
		/*sound->StopWav("Play.wav");
		GameOverInitialize();
		scene = GAMEOVER;*/
		changeColorFlag = true;
	}

	if (gameScore > 49999.0f)
	{
		targetScoreFlag = true;
	}

#pragma region 情報を取得

	playerLocalPosition = player->GetPosition();
	//playerLocalPosition = lerp(startPlayerPosition, {0.0f,0.0f,0.0f}, L5nowCount);

	playerRotation = player->GetRotation();

	// playerTargetPosition = SplineFloat3(playerTargetCheckPoint, startIndex, timeRate);

	//centerPosition = SplineFloat3(playerCheckPoint, startIndex, timeRate);


	SkydomPosition = objSkydome->GetPosition();
	SkydomRotation = objSkydome->GetRotation();

	if (bossBreak == false)
	{
		bossLocalPosition = Spline::SplineFloat3(bossCheckPoint, startIndex, timeRate);
	}
	else if (bossBreak == true)
	{
		bossLocalPosition = bossBody->GetPosition();
	}

	bossRotation = bossBody->GetRotation();

	bossLeg1LocalPosition = bossLeg1->GetPosition();
	bossLeg2LocalPosition = bossLeg2->GetPosition();
	bossLeg3LocalPosition = bossLeg3->GetPosition();
	bossLeg4LocalPosition = bossLeg4->GetPosition();

	loadingColor = LoadingBG->GetColor();

	playerHpGageSize = playerHpGage->GetSize();

	bossHpGageSize = bossHpGage->GetSize();

	playerDamageGageSize = Lerp::LerpFloat2(playerDamageGage->GetSize(), playerHpGageSize, L1timeRate);
	bossDamageGageSize = Lerp::LerpFloat2(bossDamageGage->GetSize(), bossHpGageSize, L2timeRate);

	cameraLocalPosition = Easing::OutQuadFloat3(objCamera->GetPosition(), nextCamera, L3nowCount);

	moveCameraPosition_1 = Easing::InOutQuadFloat3({ 5.0f, 0.0f,-1.0f }, { 4.0f, 3.0f,2.0f }, L5nowCount);
	moveCameraPosition_2 = Easing::InOutQuadFloat3({ 2.0f, -1.0f,-5.0f }, { -4.0f, 3.0f,-2.0f }, L5nowCount);
	moveCameraPosition_3 = Easing::InOutQuadFloat3({ 0.0f, 2.0f,-10.0f }, { 0.0f,0.0f,-20.0f }, L5nowCount);

	gameParts1Color = gameParts_1->GetColor();
	gameParts2Color = gameParts_2->GetColor();
	gameParts3Color = gameParts_3->GetColor();

	playerHpUIPosition = Easing::OutCubicFloat2(offPlayerHpUIPosition, onPlayerHpUIPosition, L4nowCount);
	bossHpUIPosition = Easing::OutCubicFloat2(offBossHpUIPosition, onBossHpUIPosition, L4nowCount);
	scoreUIPosition = Easing::OutCubicFloat2(offScoreUIPosition, onScoreUIPosition, L4nowCount);

	RankTimer();

	gameGTXT_Number1Size = Easing::OutCubicFloat2({ 320.0f, 480.0f }, { 160.0f, 240.0f }, L5nowCount);
	gameGTXT_Number1Color.w = Easing::OutCubicFloat(0.0f, 1.0f, L5nowCount);

	gameGTXT_Number2Size = Easing::OutCubicFloat2({ 320.0f, 480.0f }, { 160.0f, 240.0f }, L5nowCount);
	gameGTXT_Number2Color.w = Easing::OutCubicFloat(0.0f, 1.0f, L5nowCount);

	gameGTXT_Number3Size = Easing::OutCubicFloat2({ 320.0f, 480.0f }, { 160.0f, 240.0f }, L5nowCount);
	gameGTXT_Number3Color.w = Easing::OutCubicFloat(0.0f, 1.0f, L5nowCount);

	if (moveCameraNumber == 3)
	{
		gameGTXT_GOSize = Easing::OutCubicFloat2({ 640.0f, 480.0f }, { 320.0f, 240.0f }, L5nowCount);
		gameGTXT_GOColor.w = Easing::OutCubicFloat(0.0f, 1.0f, L5nowCount);
	}
	else if (moveCameraNumber == 4)
	{
		gameGTXT_GOColor.w = Easing::OutCubicFloat(1.0f, 0.0f, L5nowCount);
	}

	centerPosition.z += centorVel;

	if (startIndex >= 38)
	{
		bossRotation.y = 0.0f;
		startIndex = 4;
	}
#pragma endregion

	// 暗転を解除
	if (changeColorFlag == false)
	{
		if (loadingColor.w > -0.1)
		{
			loadingColor.w -= 0.05f;
		}
	}

	// 暗転させてからシーン遷移
	if (changeColorFlag == true)
	{
		L4addCount = -moveUIVel;
		loadingColor.w += 0.05f;
		changeSceneFlag = true;
	}

	if (changeSceneFlag == true)
	{
		changeSceneTimer--;
	}

	if (changeSceneTimer <= 0)
	{
		Sound::GetInstance()->StopWav("BGM/Game/game_bgm.wav");
		ResultInitialize();
		scene = RESULT;
	}

	/*if (Input::GetInstance()->TriggerKey(DIK_C))
	{
		bossHp = 0.0f;
		L2startCount = GetTickCount();
		gameScore += 49999.0f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_G))
	{
		bossLeg1Hp = 0.0f;
		bossLeg2Hp = 0.0f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_H))
	{
		bossLeg3Hp = 0.0f;
		bossLeg4Hp = 0.0f;
	}*/

	playerHpCalc();

	bossHpCalc();

	alertUIUpdate();

	// CreateCloud();

	// 雲を更新
	for (std::unique_ptr<StageObject>& cloud : stageObjects)
	{
		cloud->Update();
	}

	// 雲を消去
	stageObjects.remove_if([](std::unique_ptr<StageObject>& cloud)
		{
			return cloud->GetDeathFlag();
		}
	);

#pragma region 弾関連
	Attack();

	//chargeAttack();

	// 弾を更新
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update();
	}

	// 弾を消去
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// ボスの弾を更新
	for (std::unique_ptr<BossBullet>& bullet : bossBullets)
	{
		bullet->Update();
	}

	// ボスの弾を消去
	bossBullets.remove_if([](std::unique_ptr<BossBullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// ボスの狙い弾を更新
	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		bullet->Update();
	}

	// ボスの狙い弾を消去
	bossTargetBullets.remove_if([](std::unique_ptr<BossTargetBullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);
#pragma endregion

#pragma region 座標変換
	// カメラの座標変換
	XMVECTOR cameraPositionV;

	cameraPositionV = DirectX::XMLoadFloat3(&objCamera->GetPosition());
	cameraPositionV.m128_f32[3] = 1.0f;
	cameraPositionV = DirectX::XMVector3Transform(cameraPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&cameraWorldPosition, cameraPositionV);

	// プレイヤーの座標変換
	XMVECTOR playerPositionV;

	playerPositionV = DirectX::XMLoadFloat3(&playerLocalPosition);
	playerPositionV.m128_f32[3] = 1.0f;
	playerPositionV = DirectX::XMVector3Transform(playerPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&playerWorldPosition, playerPositionV);

	// プレイヤー飛行機雲(右)の座標変換
	XMVECTOR playerContrailRightPositionV;

	playerContrailRightPositionV = DirectX::XMLoadFloat3(&objPlayerContrailRight->GetPosition());
	playerContrailRightPositionV.m128_f32[3] = 1.0f;
	playerContrailRightPositionV = DirectX::XMVector3Transform(playerContrailRightPositionV, player->GetMatWorld());
	DirectX::XMStoreFloat3(&playerContrailRightWorldPosition, playerContrailRightPositionV);

	// プレイヤー飛行機雲(左)の座標変換
	XMVECTOR playerContrailLeftPositionV;

	playerContrailLeftPositionV = DirectX::XMLoadFloat3(&objPlayerContrailLeft->GetPosition());
	playerContrailLeftPositionV.m128_f32[3] = 1.0f;
	playerContrailLeftPositionV = DirectX::XMVector3Transform(playerContrailLeftPositionV, player->GetMatWorld());
	DirectX::XMStoreFloat3(&playerContrailLeftWorldPosition, playerContrailLeftPositionV);

	// ボスの座標変換
	XMVECTOR bossBodyPositionV;
	XMVECTOR bossLeg1PositionV;
	XMVECTOR bossLeg2PositionV;
	XMVECTOR bossLeg3PositionV;
	XMVECTOR bossLeg4PositionV;

	bossBodyPositionV = DirectX::XMLoadFloat3(&bossLocalPosition);
	bossBodyPositionV.m128_f32[3] = 1.0f;
	bossBodyPositionV = DirectX::XMVector3Transform(bossBodyPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&bossWorldPosition, bossBodyPositionV);

	bossLeg1PositionV = DirectX::XMLoadFloat3(&bossLeg1LocalPosition);
	bossLeg1PositionV.m128_f32[3] = 1.0f;
	bossLeg1PositionV = DirectX::XMVector3Transform(bossLeg1PositionV, bossLowerBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg1WorldPosition, bossLeg1PositionV);

	bossLeg2PositionV = DirectX::XMLoadFloat3(&bossLeg2LocalPosition);
	bossLeg2PositionV.m128_f32[3] = 1.0f;
	bossLeg2PositionV = DirectX::XMVector3Transform(bossLeg2PositionV, bossLowerBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg2WorldPosition, bossLeg2PositionV);

	bossLeg3PositionV = DirectX::XMLoadFloat3(&bossLeg3LocalPosition);
	bossLeg3PositionV.m128_f32[3] = 1.0f;
	bossLeg3PositionV = DirectX::XMVector3Transform(bossLeg3PositionV, bossLowerBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg3WorldPosition, bossLeg3PositionV);

	bossLeg4PositionV = DirectX::XMLoadFloat3(&bossLeg4LocalPosition);
	bossLeg4PositionV.m128_f32[3] = 1.0f;
	bossLeg4PositionV = DirectX::XMVector3Transform(bossLeg4PositionV, bossLowerBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg4WorldPosition, bossLeg4PositionV);
#pragma endregion

#pragma region ボス関連

#pragma region ボスの行動パターン
	switch (bossPattern)
	{
	case BOSSPATTERN::STAY:


		break;

	case BOSSPATTERN::NORMAL:
		bossBody->shotTimer--;

		if (bossBody->shotTimer <= 0)
		{
			BossAttack();
			bossBody->shotTimer = bossBody->ShotInterval;
		}
		break;

	case BOSSPATTERN::BODYUP:
		rushFlag = true;
		if (bossRotation.x <= 90.0f)
		{
			bossRotation.x += 1.0f;
		}
		else if (bossRotation.x >= 90.0f)
		{
			bossPattern = RUSH;
		}

		break;

	case BOSSPATTERN::BODYDOWN:
		if (bossRotation.x >= 0.0f)
		{
			bossRotation.x -= 1.0f;
		}
		else if (bossRotation.x <= 0.0f)
		{
			rushFlag = false;
			bossPattern = STAY;
		}

		break;

	case BOSSPATTERN::RUSH:
		bossBody->shotTimer--;

		if (bossBody->shotTimer <= 0)
		{
			if (rushOrder >= 4)
			{
				rushOrder = 0;
			}

			if (rushOrder == 0)
			{
				BossLeg1Attack();
			}
			if (rushOrder == 1)
			{
				BossLeg2Attack();
			}
			if (rushOrder == 2)
			{
				BossLeg4Attack();
			}
			if (rushOrder == 3)
			{
				BossLeg3Attack();
			}

			rushOrder += 1;
			bossBody->shotTimer = bossBody->RushInterval;
		}
		break;

	case BOSSPATTERN::BODYRIGHT:

		if (bossRotation.y <= bossRotateVel)
		{
			bossRotation.y += 1.0f;
		}

		break;

	case BOSSPATTERN::BODYLEFT:

		if (bossRotation.y >= -bossRotateVel)
		{
			bossRotation.y -= 1.0f;
		}

		break;

	case BOSSPATTERN::FOURWAYRUSH:

		break;
	}
#pragma endregion

#pragma region 特定のルート通過でパターン変化
	if (startIndex == 5)
	{
		bossPattern = NORMAL;
	}
	else if (startIndex == 11)
	{
		bossPattern = STAY;
	}
	else if (startIndex == 13)
	{
		bossRotateVel = 90.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 14)
	{
		bossRotateVel = 180.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 16)
	{
		bossPattern = BODYUP;
	}
	else if (startIndex == 20)
	{
		bossPattern = BODYDOWN;
	}
	else if (startIndex == 30)
	{
		bossRotateVel = 270.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 31)
	{
		bossRotateVel = 360.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 32)
	{
		bossPattern = BODYUP;
	}
	else if (startIndex == 36)
	{
		bossPattern = BODYDOWN;
	}
#pragma endregion

#pragma region 当たり判定
	// ボス本体の当たり判定
	if (bossFlag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossWorldPosition, 0.7f, 0.6f) == true)
			{
				bossHp -= 10.0f;
				L2startCount = GetTickCount();
				gameScore += 1000.0f * scoreRate;
				scoreUIMotion();
				scoreRateCount++;
				Sound::GetInstance()->PlayWav("SE/Game/game_boss_damage.wav", seVolume);
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossWorldPosition);
			}
		}
	}

	if (bossHp <= 0.0f)
	{
		bossBreak = true;
	}

	if (bossBreak == true)
	{
		bossLocalPosition.y -= 0.3f;
		CreateBossParticles(bossWorldPosition);
	}

	if (bossLocalPosition.y <= -15.0f)
	{
		// bossBreak == false;

		bossFlag = false;
		bossLeg1Flag = false;
		bossLeg2Flag = false;
		bossLeg3Flag = false;
		bossLeg4Flag = false;
	}

	// ボス部位1の当たり判定
	if (bossLeg1Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg1WorldPosition, 0.6f, 0.6f) == true)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_boss_damage.wav", seVolume);
				bossHp -= 5.0f;
				L2startCount = GetTickCount();
				gameScore += 250.0f * scoreRate;
				scoreUIMotion();
				scoreRateCount++;
				if (rushFlag == false)
				{
					bossLeg1Hp -= 1.0f;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg1WorldPosition);
			}
		}
	}

	if (bossLeg1Hp <= 0.0f)
	{
		bossLeg1Break = true;
		bossLeg1DeathFlag = true;
	}


	if (bossLeg1Break == true)
	{
		bossLeg1LocalPosition.y -= 0.2f;
		if (bossLeg1Flag == true)
		{
			CreateBossParticles(bossLeg1WorldPosition);
		}
	}

	if (bossLeg1LocalPosition.y <= -20.0f)
	{
		bossLeg1Break = false;
		bossLeg1Flag = false;
	}

	// ボス部位2の当たり判定
	if (bossLeg2Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg2WorldPosition, 0.6f, 0.6f) == true)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_boss_damage.wav", seVolume);
				bossHp -= 5.0f;
				L2startCount = GetTickCount();
				gameScore += 250.0f * scoreRate;
				scoreUIMotion();
				scoreRateCount++;
				if (rushFlag == false)
				{
					bossLeg2Hp -= 1.0f;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg2WorldPosition);
			}
		}
	}

	if (bossLeg2Hp <= 0.0f)
	{
		bossLeg2Break = true;
		bossLeg2DeathFlag = true;
	}


	if (bossLeg2Break == true)
	{
		bossLeg2LocalPosition.y -= 0.2f;
		if (bossLeg2Flag == true)
		{
			CreateBossParticles(bossLeg2WorldPosition);
		}
	}

	if (bossLeg2LocalPosition.y <= -20.0f)
	{
		bossLeg2Break = false;
		bossLeg2Flag = false;
	}

	// ボス部位3の当たり判定
	if (bossLeg3Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg3WorldPosition, 0.6f, 0.6f) == true)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_boss_damage.wav", seVolume);
				bossHp -= 5.0f;
				L2startCount = GetTickCount();
				gameScore += 250.0f * scoreRate;
				scoreUIMotion();
				scoreRateCount++;
				if (rushFlag == false)
				{
					bossLeg3Hp -= 1.0f;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg3WorldPosition);
			}
		}
	}

	if (bossLeg3Hp <= 0.0f)
	{
		bossLeg3Break = true;
		bossLeg3DeathFlag = true;
	}


	if (bossLeg3Break == true)
	{
		bossLeg3LocalPosition.y -= 0.2f;
		if (bossLeg3Flag == true)
		{
			CreateBossParticles(bossLeg3WorldPosition);
		}

	}

	if (bossLeg3LocalPosition.y <= -20.0f)
	{
		bossLeg3Break = false;
		bossLeg3Flag = false;
	}

	// ボス部位4の当たり判定
	if (bossLeg4Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg4WorldPosition, 0.6f, 0.6f) == true)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_boss_damage.wav", seVolume);
				bossHp -= 5.0f;
				L2startCount = GetTickCount();
				gameScore += 250.0f * scoreRate;
				scoreUIMotion();
				scoreRateCount++;
				if (rushFlag == false)
				{
					bossLeg4Hp -= 1.0f;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg4WorldPosition);
			}
		}
	}

	if (bossLeg4Hp <= 0.0f)
	{
		bossLeg4Break = true;
		bossLeg4DeathFlag = true;
	}

	if (bossLeg4Break == true)
	{
		bossLeg4LocalPosition.y -= 0.2f;
		if (bossLeg4Flag == true)
		{
			CreateBossParticles(bossLeg4WorldPosition);
		}
	}

	if (bossLeg4LocalPosition.y <= -20.0f)
	{
		bossLeg4Break = false;
		bossLeg4Flag = false;
	}

	if ((bossLeg1DeathFlag == true) && (bossLeg2DeathFlag == true) && (bossLeg3DeathFlag == true) && (bossLeg4DeathFlag == true))
	{
		allLegBreakFlag = true;
	}

	// プレイヤーの当たり判定
	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		if (OnCollision(bullet->GetPosition(), playerWorldPosition, 0.6f, 0.6f) == true)
		{
			damageEffectAlpha = 1.0f;
			damageEffectAlphaVel = -0.06f;
			playerHp -= 10.0f;
			if (scoreRate == 1.5f)
			{
				scoreRateTimer = 0;
				scoreRateCount = 0;
			}
			else if (scoreRate == 2.0f)
			{
				scoreRateTimer = 0;
				scoreRateCount = 20;
			}
			L1startCount = GetTickCount();
			noDamageFlag = false;
			Sound::GetInstance()->PlayWav("SE/Game/game_player_damage.wav", seVolume);
			bullet->deathFlag = true;
			// パーティクル生成
			CreateHitParticles(playerWorldPosition);
		}
	}

#pragma endregion

#pragma endregion

	if (player->dodgeRollFlag)
	{
		CreatePlayerContrailParticles(playerContrailRightWorldPosition);

		CreatePlayerContrailParticles(playerContrailLeftWorldPosition);
	}

	damageEffectUpdate();

	changeGameUIAlpha();

	ScoreRateTimer();

	if (scoreRateCount >= 50)
	{
		scoreRate = 2.0f;
	}
	else if (scoreRateCount >= 20)
	{
		scoreRate = 1.5f;
	}
	else
	{
		scoreRate = 1.0f;
	}

#pragma region スプライン曲線関係
	if (railCountFlag == true)
	{
		SplineCount();
	}

	Lerp1Count();
	Lerp2Count();
	Lerp3Count();
	Lerp4Count();
	Lerp5Count();

#pragma endregion

	railTargetCalc();

	CreatePlayerJetParticles(playerWorldPosition);

	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		CreatePlayerBulletParticles(bullet->GetPosition());
	}

	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		CreateBossBulletParticles(bullet->GetPosition());
	}

#pragma region 座標のセット
	// カメラ座標のセット
	CameraSwitching();
	camera->SetTarget(centerPosition);

	// プレイヤー座標のセット
	player->SetPosition(playerLocalPosition);
	// player->SetRotation({playerRotation.x, -testDegrees + 180.0f, playerRotation.z});
	player->SetRotation(playerRotation);

	// レール中心オブジェクト座標のセット
	objCenter->SetPosition(centerPosition);
	// objCenter->SetRotation({ centerRotation.x, -testDegrees + 90.0f, centerRotation.z });
	objCenter->SetRotation({ centerRotation.x, centerRotation.y, centerRotation.z });

	// 背景天球座標のセット
	objSkydome->SetPosition(centerPosition);

	// ボス関連座標のセット
	bossBody->SetPosition(bossLocalPosition);
	bossBody->SetRotation(bossRotation);

	bossLeg1->SetPosition(bossLeg1LocalPosition);
	bossLeg2->SetPosition(bossLeg2LocalPosition);
	bossLeg3->SetPosition(bossLeg3LocalPosition);
	bossLeg4->SetPosition(bossLeg4LocalPosition);

	LoadingBG->SetColor(loadingColor);

	// ダメージエフェクト
	damageEffect->SetColor({ 1.0f, 1.0f, 1.0f, damageEffectAlpha });

	// スコアUI
	scoreUI->SetPosition(scoreUIPosition);
	scoreUI->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreChar->SetPosition({ 190.0f, scoreUIPosition.y - 32.0f });
	scoreChar->SetColor({ 0.760f, 0.929f, 1.0f, scoreUIAlpha });

	scoreRateChar->SetPosition({ 210.0f, scoreUIPosition.y + 12.0f });
	scoreRateChar->SetColor({ 1.0f, 1.0f, 0.4f, scoreUIAlpha });

	scoreNull_1->SetPosition({ 403.5f, scoreUIPosition.y - 33.0f });
	scoreNull_1->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreNull_2->SetPosition({ 378.0f, scoreUIPosition.y - 33.0f });
	scoreNull_2->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreNull_3->SetPosition({ 352.5f, scoreUIPosition.y - 33.0f });
	scoreNull_3->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreNull_4->SetPosition({ 326.5f, scoreUIPosition.y - 33.0f });
	scoreNull_4->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreNull_5->SetPosition({ 300.5f, scoreUIPosition.y - 33.0f });
	scoreNull_5->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	scoreNull_6->SetPosition({ 275.0f, scoreUIPosition.y - 33.0f });
	scoreNull_6->SetColor({ 1.0f, 1.0f, 1.0f, scoreUIAlpha });

	// プレイヤーのHPゲージ
	playerHpGage->SetPosition(playerHpUIPosition);
	playerHpGage->SetColor({ 0.1f, 0.6f, 0.1f, playerHpUIAlpha });
	playerHpGage->SetSize(playerHpGageSize);

	playerDamageGage->SetPosition(playerHpUIPosition);
	playerDamageGage->SetColor({ 1.0f, 0, 0.2f, playerDamageUIAlpha });
	playerDamageGage->SetSize(playerDamageGageSize);

	playerHpUI->SetPosition({ playerHpUIPosition.x + 10.0f, playerHpUIPosition.y });
	playerHpUI->SetColor({ 1.0f, 1.0f, 1.0f, playerHpUIAlpha });

	playerHpUICover->SetPosition({ playerHpUIPosition.x + 10.0f, playerHpUIPosition.y });
	playerHpUICover->SetColor({ 1.0f, 1.0f, 1.0f, playerHpUIAlpha });

	// ボスのHPゲージ
	bossHpGage->SetPosition(bossHpUIPosition);
	bossHpGage->SetColor({ 0.1f, 0.6f, 0.1f, bossHpUIAlpha });
	bossHpGage->SetSize(bossHpGageSize);

	bossDamageGage->SetPosition(bossHpUIPosition);
	bossDamageGage->SetColor({ 1.0f, 0, 0.2f, bossDamageUIAlpha });
	bossDamageGage->SetSize(bossDamageGageSize);

	bossHpUI->SetPosition({ bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });
	bossHpUI->SetColor({ 1.0f, 1.0f, 1.0f, bossHpUIAlpha });

	bossHpUICover->SetPosition({ bossHpUIPosition.x + 10.0f, bossHpUIPosition.y });
	bossHpUICover->SetColor({ 1.0f, 1.0f, 1.0f, bossHpUIAlpha });

	gameGTXT_1->SetPosition({ 66.0f,scoreUIPosition.y + 2.0f });
	gameGTXT_1->SetSize({ gameGTXT_1Size });
	gameGTXT_1->SetColor({ 1.0f, 1.0f, 0.0f, scoreUIAlpha - gameGTXT_1Color.w });

	gameGTXT_2->SetPosition({ 66.0f,scoreUIPosition.y + 2.0f });
	gameGTXT_2->SetSize({ gameGTXT_2Size });
	gameGTXT_2->SetColor({ 1.0f, 0.4f, 0.0f, scoreUIAlpha - gameGTXT_2Color.w });

	gameGTXT_3->SetPosition({ 66.0f,scoreUIPosition.y + 2.0f });
	gameGTXT_3->SetSize({ gameGTXT_3Size });
	gameGTXT_3->SetColor({ 0.0f, 1.0f, 1.0f, scoreUIAlpha - gameGTXT_3Color.w });

	gameGTXT_4->SetPosition({ 66.0f,scoreUIPosition.y + 2.0f });
	gameGTXT_4->SetSize({ gameGTXT_4Size });
	gameGTXT_4->SetColor({ 0.0f, 1.0f, 0.0f, scoreUIAlpha - gameGTXT_4Color.w });

	gameParts_1->SetColor(gameParts1Color);
	gameParts_2->SetColor(gameParts2Color);
	gameParts_3->SetColor(gameParts3Color);

	gameGTXT_number1->SetSize(gameGTXT_Number1Size);
	gameGTXT_number1->SetColor(gameGTXT_Number1Color);

	gameGTXT_number2->SetSize(gameGTXT_Number2Size);
	gameGTXT_number2->SetColor(gameGTXT_Number2Color);

	gameGTXT_number3->SetSize(gameGTXT_Number3Size);
	gameGTXT_number3->SetColor(gameGTXT_Number3Color);

	gameGTXT_GO->SetSize(gameGTXT_GOSize);
	gameGTXT_GO->SetColor(gameGTXT_GOColor);
#pragma endregion

#pragma region 更新処理
	// ボス関連の更新
	bossBody->Update();
	bossUpperBody->Update();
	bossLowerBody->Update();
	bossLeg1->Update();
	bossLeg2->Update();
	bossLeg3->Update();
	bossLeg4->Update();

	// パーティクルの更新
	bossHitParticle->Update();
	bossBreakParticle->Update();
	playerJetParticle->Update();
	playerContrailParticle->Update();
	playerBulletParticle->Update();
	// 背景天球
	objSkydome->Update();

	// 地面の更新
	objGround->Update();

	// カメラの更新
	camera->Update();

	// プレイヤーの更新
	player->Update();
	objPlayerContrailRight->Update();
	objPlayerContrailLeft->Update();

	objCamera->Update();

	// レール中心オブジェクトの更新
	objCenter->Update();

	objCloud_1->Update();

	// FBXの更新
	testobject->Update();

	if (Input::GetInstance()->TriggerKey(DIK_I) && helpMenuFlag == false)
	{
		helpMenuFlag = true;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_I) && helpMenuFlag == true)
	{
		helpMenuFlag = false;
	}

	if (helpMenuFlag == false)
	{

	}

#pragma endregion

	collisionManager->CheckAllCollisions();

	// デバックテキスト
	AllDebugText();
	GameDebugText();

	// スコアの描画
	std::ostringstream StartIndex;
	StartIndex << std::fixed << std::setprecision(0) << std::setw(7) << gameScore;
	scoreText.Print(StartIndex.str(), { 300.0f , scoreUIPosition.y + -7.0f + scoreBasePosition.y }, { 0.760f, 0.929f, 1.0f, scoreUIAlpha }, 0.8f);

	std::ostringstream ScoreRate;
	ScoreRate << "x" << std::fixed << std::setprecision(0) << std::setprecision(1) << scoreRate;
	scoreText.Print(ScoreRate.str(), { 300.0f , scoreUIPosition.y + 23.0f }, { 1.0f, 1.0f, 0.4f, scoreUIAlpha - scoreRateAlpha }, 0.6f);

	scoreUIUpdate();
}

void GameScene::GameDraw()
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
	player->Draw();
	/*objPlayerContrailRight->Draw();
	objPlayerContrailLeft->Draw();*/

	if (playerUpdateFlag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			//bullet->Draw();
		}
	}

	for (std::unique_ptr<BossBullet>& bullet : bossBullets)
	{
		//bullet->Draw();
	}

	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		//bullet->Draw();
	}

	for (std::unique_ptr<StageObject>& cloud : stageObjects)
	{
		cloud->Draw();
	}

	if (bossFlag == true)
	{
		bossBody->Draw();
		bossUpperBody->Draw();
		bossLowerBody->Draw();

		if (bossLeg1Flag == true)
		{
			bossLeg1->Draw();
		}

		if (bossLeg2Flag == true)
		{
			bossLeg2->Draw();
		}

		if (bossLeg3Flag == true)
		{
			bossLeg3->Draw();
		}

		if (bossLeg4Flag == true)
		{
			bossLeg4->Draw();
		}
	}

	// testobject->Draw(cmdList);

	// objCenter->Draw();

	objCloud_1->Draw();

	//objCamera->Draw();

	// パーティクルの描画
	bossHitParticle->Draw(cmdList);
	bossBreakParticle->Draw(cmdList);
	playerJetParticle->Draw(cmdList);
	playerContrailParticle->Draw(cmdList);
	if (playerUpdateFlag == true)
	{
		playerBulletParticle->Draw(cmdList);
	}

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	damageEffect->Draw();

	// 描画
	playerHpUI->Draw();
	playerDamageGage->Draw();
	playerHpGage->Draw();
	playerHpUICover->Draw();

	bossHpUI->Draw();
	bossDamageGage->Draw();
	bossHpGage->Draw();
	bossHpUICover->Draw();

	scoreUI->Draw();
	scoreChar->Draw();
	scoreRateChar->Draw();

	//GameFG->Draw();

	if (backFlashingFlag == true)
	{
		gameParts_1->Draw();
	}

	if (rightFlashingFlag == true)
	{
		gameParts_2->Draw();
	}

	if (leftFlashingFlag == true)
	{
		gameParts_3->Draw();
	}

	// 
	if (moveCameraNumber == 0)
	{
		gameGTXT_number3->Draw();
	}
	if (moveCameraNumber == 1)
	{
		gameGTXT_number2->Draw();
	}
	if (moveCameraNumber == 2)
	{
		gameGTXT_number1->Draw();
	}
	if (moveCameraNumber >= 3)
	{
		gameGTXT_GO->Draw();
	}

	// スコアテキストの描画
	scoreText.DrawAll(cmdList);

	if (gameScore < 10.0f)
	{
		scoreNull_1->Draw();
	}
	if (gameScore < 100.0f)
	{
		scoreNull_2->Draw();
	}
	if (gameScore < 1000.0f)
	{
		scoreNull_3->Draw();
	}
	if (gameScore < 10000.0f)
	{
		scoreNull_4->Draw();
	}
	if (gameScore < 100000.0f)
	{
		scoreNull_5->Draw();
	}
	if (gameScore < 1000000.0f)
	{
		scoreNull_6->Draw();
	}

	if (gameScore > 99999)
	{
		// S
		gameGTXT_1->Draw();
	}
	else if (gameScore > 49999)
	{
		// A
		gameGTXT_2->Draw();
	}
	else if (gameScore > 24999)
	{
		// B
		gameGTXT_3->Draw();
	}
	else
	{
		// C
		gameGTXT_4->Draw();
	}

	LoadingBG->Draw();

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::GameInitialize()
{
	// 座標のセット
	player->SetPosition({ 0.0f,0.0f,0.0f });
	player->SetRotation({ 0.0f, 90.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	objPlayerContrailRight->SetPosition({ 1.3f, 0.1f,1.7f });
	objPlayerContrailRight->SetScale({ 0.1f, 0.1f, 0.1f });

	objPlayerContrailLeft->SetPosition({ 1.3f, 0.1f,-1.7f });
	objPlayerContrailLeft->SetScale({ 0.1f, 0.1f, 0.1f });

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	objCamera->SetPosition({ 0.0f,0.0f,-20.0f });

	objCenter->SetPosition({ 0.0f,0.0f,0.0f });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	bossBody->SetPosition({ 0.0f,0.0f,20.0f });
	bossUpperBody->SetPosition({ 0.0f, 0.2f, 0.0f });
	bossLowerBody->SetPosition({ 0.0f, -0.2f, 0.0f });
	bossLeg1->SetPosition({ 1.5f,-1.5f,1.5f });
	bossLeg2->SetPosition({ 1.5f,-1.5f,-1.5f });
	bossLeg3->SetPosition({ -1.5f,-1.5f,1.5f });
	bossLeg4->SetPosition({ -1.5f,-1.5f,-1.5f });

	bossBody->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg1->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg2->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg3->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg4->SetRotation({ 0.0f,0.0f,0.0f });

	bossBody->SetScale({ 2.5f, 2.5f, 2.5f });
	bossLeg1->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg2->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg3->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg4->SetScale({ 0.8f, 0.8f, 0.8f });

	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetEye({ 0.0f, 0.0f, 10.0f });
	camera->SetUp({ 0.0f, 1.0f, 0.0f });

	objCloud_1->SetPosition({ 30.0f,0.0f,100.0f });
	objCloud_1->SetRotation({ 0.0f,180.0f,0.0f });

	// シーン遷移時の画面暗転
	//LoadingBG->SetColor({ 1, 1, 1, 1.0f });
	loadingColor.w = 1.0f;

	// プレイヤーのHPゲージ
	playerHpGage->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	playerHpGage->SetSize({ 320.0f, 30.0f });
	playerHpGage->SetAnchorPoint({ 1.0f, 0.5f });

	playerDamageGage->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	playerDamageGage->SetSize({ 320.0f, 30.0f });
	playerDamageGage->SetAnchorPoint({ 1.0f, 0.5f });

	// playerHpUI->SetColor({ 1.0, 0.2, 0.2, 1});
	playerHpUI->SetAnchorPoint({ 1.0f,0.5f });

	playerHpUICover->SetAnchorPoint({ 1.0f, 0.5f });
	//playerHpUICover->SetColor({ 1.0, 0.2, 0.2, 1 });

	// ボスのHPゲージ
	bossHpGage->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	bossHpGage->SetSize({ 530.0f, 30.0f });
	bossHpGage->SetAnchorPoint({ 1.0f, 0.5f });

	bossDamageGage->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	bossDamageGage->SetSize({ 530.0f, 30.0f });
	bossDamageGage->SetAnchorPoint({ 1.0f, 0.5f });

	bossHpUI->SetAnchorPoint({ 1.0f, 0.5f });

	bossHpUICover->SetAnchorPoint({ 1.0f, 0.5f });

	// スコアUI
	scoreUI->SetAnchorPoint({ 0.0f, 0.5f });

	scoreChar->SetColor({ 0.760f, 0.929f, 1.0f, 1.0f });

	scoreRateChar->SetColor({ 1.0f, 1.0f, 0.4f, 1.0f });
	scoreRateChar->SetAnchorPoint({ 0.5f, 0.5f });

	scoreNull_1->SetSize({ 25.0f, 25.0f });
	scoreNull_2->SetSize({ 25.0f, 25.0f });
	scoreNull_3->SetSize({ 25.0f, 25.0f });
	scoreNull_4->SetSize({ 25.0f, 25.0f });
	scoreNull_5->SetSize({ 25.0f, 25.0f });
	scoreNull_6->SetSize({ 25.0f, 25.0f });

	scoreBasePosition = { 0.0f, 0.0f };

	gameParts_1->SetAnchorPoint({ 0.5f, 0.5f });
	gameParts_1->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	gameParts_2->SetRotation({ 270.0f });
	gameParts_2->SetAnchorPoint({ 0.5f, 0.5f });
	gameParts_2->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	gameParts_3->SetRotation({ 90.0f });
	gameParts_3->SetAnchorPoint({ 0.5f, 0.5f });
	gameParts_3->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	playerHpUIPosition = { 1154.0f, 680.0f };

	bossHpUIPosition = { 1255.0f , 30.0f };

	scoreUIPosition = { 15.0f, 60.0f };

	gameGTXT_1->SetSize({ 60.0f, 60.0f });
	gameGTXT_1->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	gameGTXT_1->SetAnchorPoint({ 0.5f, 0.5f });

	gameGTXT_2->SetSize({ 60.0f, 60.0f });
	gameGTXT_2->SetColor({ 1.0f, 0.4f, 0.0f, 1.0f });
	gameGTXT_2->SetAnchorPoint({ 0.5f, 0.5f });

	gameGTXT_3->SetSize({ 60.0f, 60.0f });
	gameGTXT_3->SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });
	gameGTXT_3->SetAnchorPoint({ 0.5f, 0.5f });

	gameGTXT_4->SetSize({ 60.0f, 60.0f });
	gameGTXT_4->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	gameGTXT_4->SetAnchorPoint({ 0.5f, 0.5f });

	gameGTXT_number1->SetPosition({ 640.0f, 360.0f });
	gameGTXT_number1->SetSize({ 160.0f, 240.0f });
	gameGTXT_number1->SetAnchorPoint({ 0.5f, 0.5f });
	gameGTXT_number1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	gameGTXT_number2->SetPosition({ 640.0f, 360.0f });
	gameGTXT_number2->SetSize({ 160.0f, 240.0f });
	gameGTXT_number2->SetAnchorPoint({ 0.5f, 0.5f });
	gameGTXT_number2->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	gameGTXT_number3->SetPosition({ 640.0f, 360.0f });
	gameGTXT_number3->SetSize({ 160.0f, 240.0f });
	gameGTXT_number3->SetAnchorPoint({ 0.5f, 0.5f });
	gameGTXT_number3->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	gameGTXT_GO->SetPosition({ 640.0f, 360.0f });
	gameGTXT_GO->SetSize({ 320.0f, 240.0f });
	gameGTXT_GO->SetAnchorPoint({ 0.5f, 0.5f });
	gameGTXT_GO->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	// プレイヤー関連
	playerHpMax = 300.0f;
	playerHp = playerHpMax;

	// ボス関連
	bossHpMax = 800.0f;
	bossHp = bossHpMax;
	bossLeg1Hp = 20.0f;
	bossLeg2Hp = 20.0f;
	bossLeg3Hp = 20.0f;
	bossLeg4Hp = 20.0f;

	bossBreak = false;
	bossLeg1Break = false;
	bossLeg2Break = false;
	bossLeg3Break = false;
	bossLeg4Break = false;

	bossFlag = true;
	bossLeg1Flag = true;
	bossLeg2Flag = true;
	bossLeg3Flag = true;
	bossLeg4Flag = true;

	rushFlag = false;

	// デスフラグ
	bossDeathFlag = false;
	bossLeg1DeathFlag = false;
	bossLeg2DeathFlag = false;
	bossLeg3DeathFlag = false;
	bossLeg4DeathFlag = false;

	cameraMode = 0;

	cameraType = START;

	playerUpdateFlag = false;

	bossPattern = STAY;

	rushOrder = 0;

	shotFlag = false;

	shotRate = 1.5f;

	startIndex = 1;

	changeColorFlag = false;
	changeColorTimer = 30.0f;

	changeSceneFlag = false;
	changeSceneTimer = 100.0f;

	gameScore = 0;

	gameScoreMax = 9999999.0f;

	scoreRateCount = 0;

	noDamageFlag = true;
	targetScoreFlag = false;
	allLegBreakFlag = false;

	damageEffectAlpha = 0.0f;
	damageEffectAlphaVel = 0.0f;

	scoreUIAlpha = 1.0f;
	scoreUIAlphaVel = 0.0f;

	playerHpUIAlpha = 1.0f;
	playerHpUIAlphaVel = 0.0f;

	playerDamageUIAlpha = 1.0f;
	playerDamageUIAlphaVel = 0.0f;

	bossHpUIAlpha = 1.0f;
	bossHpUIAlphaVel = 0.0f;

	bossDamageUIAlpha = 1.0f;
	bossDamageUIAlphaVel = 0.0f;

	moveCameraNumber = 0;

	rankSOneTimeFlag = true;
	rankAOneTimeFlag = true;
	rankBOneTimeFlag = true;
	rankCOneTimeFlag = true;

	rate1stOneTimeFlag = true;
	rate2ndOneTimeFlag = true;
	rate3rdOneTimeFlag = true;

	//railCountFlag = false;

	Sound::GetInstance()->PlayWav("BGM/Game/game_bgm.wav", bgmVolume, true);

	startCount = GetTickCount();
}

void GameScene::ResultUpdate()
{
	loadingColor = LoadingBG->GetColor();

	resultBN_1Size = ResultBN_1->GetSize();
	resultBN_2Size = ResultBN_2->GetSize();
	resultBN_3Size = ResultBN_3->GetSize();

	resultParts_1Size = resultParts_1->GetSize();
	resultParts_2Size = resultParts_2->GetSize();
	resultParts_3Size = resultParts_3->GetSize();

	missionStar1Size = resultParts_7->GetSize();
	missionStar2Size = resultParts_8->GetSize();
	missionStar3Size = resultParts_9->GetSize();

	rankSSize = resultGTXT_6->GetSize();
	rankASize = resultGTXT_7->GetSize();
	rankBSize = resultGTXT_8->GetSize();
	rankCSize = resultGTXT_9->GetSize();

	resultGTXT_15Size = resultGTXT_15->GetSize();

	resultColor = resultGTXT_15->GetColor();
	resultPartsColor = resultParts_10->GetColor();
	scoreColor = resultGTXT_1->GetColor();
	noDamageBonusColor = resultGTXT_2->GetColor();
	totalScoreColor = resultGTXT_3->GetColor();
	rankColor = resultGTXT_4->GetColor();
	missionColor = resultGTXT_5->GetColor();

	rankSColor = resultGTXT_6->GetColor();
	rankAColor = resultGTXT_7->GetColor();
	rankBColor = resultGTXT_8->GetColor();
	rankCColor = resultGTXT_9->GetColor();

	mission1Color = resultGTXT_12->GetColor();
	mission2Color = resultGTXT_13->GetColor();
	mission3Color = resultGTXT_14->GetColor();

	missionStar1Color = resultParts_4->GetColor();
	missionStar2Color = resultParts_5->GetColor();
	missionStar3Color = resultParts_6->GetColor();
	missionStar4Color = resultParts_7->GetColor();
	missionStar5Color = resultParts_8->GetColor();
	missionStar6Color = resultParts_9->GetColor();

	/*if (changeColorFlag == false)
	{
		if (loadingColor.w > -0.1)
		{
			loadingColor.w -= 0.05f;
		}
	}*/

	// 合計スコアの計算
	if (noDamageFlag == false)
	{
		noDamageBonus = 0;
	}

	totalScore = gameScore + noDamageBonus;

	// サイズ値の計算
	resultMoveVelX_1 -= resultMoveAccX_1;
	resultBN_1Size.x += resultMoveVelX_1;

	resultMoveVelY_1 -= resultMoveAccY_1;
	resultBN_1Size.y += resultMoveVelY_1;

	resultMoveVelX_2 -= resultMoveAccX_2;
	resultBN_2Size.x += resultMoveVelX_2;

	resultMoveVelX_3 -= resultMoveAccX_3;
	resultBN_3Size.x += resultMoveVelX_3;

	resultMoveVelX_4 -= resultMoveAccX_4;
	resultParts_1Size.x += resultMoveVelX_4;

	resultMoveVelX_5 -= resultMoveAccX_5;
	resultParts_2Size.x += resultMoveVelX_5;

	resultMoveVelX_6 -= resultMoveAccX_6;
	resultParts_3Size.x += resultMoveVelX_6;

	rankSSize.x += resultMoveVel_8;
	rankSSize.y += resultMoveVel_8;

	rankASize.x += resultMoveVel_8;
	rankASize.y += resultMoveVel_8;

	rankBSize.x += resultMoveVel_8;
	rankBSize.y += resultMoveVel_8;

	rankCSize.x += resultMoveVel_8;
	rankCSize.y += resultMoveVel_8;

	missionStar1Size.x += resultMoveVel_9;
	missionStar1Size.y += resultMoveVel_9;

	missionStar2Size.x += resultMoveVel_10;
	missionStar2Size.y += resultMoveVel_10;

	missionStar3Size.x += resultMoveVel_11;
	missionStar3Size.y += resultMoveVel_11;

	//
	resultGTXT_15Size.y += resultMoveVel_7;
	resultGTXT_15Size.x += resultMoveVel_7 * 5.5f;

	// アルファ値の計算
	resultColor.w += resultChangeAlpha_6;
	resultPartsColor.w += resultChangeAlpha_7;

	scoreColor.w += resultChangeAlpha_1;

	noDamageBonusColor.w += resultChangeAlpha_2;

	totalScoreColor.w += resultChangeAlpha_3;

	rankColor.w += resultChangeAlpha_4;

	rankSColor.w += resultChangeAlpha_8;
	rankAColor.w += resultChangeAlpha_8;
	rankBColor.w += resultChangeAlpha_8;
	rankCColor.w += resultChangeAlpha_8;

	missionColor.w += resultChangeAlpha_5;
	mission1Color.w += resultChangeAlpha_5;
	mission2Color.w += resultChangeAlpha_5;
	mission3Color.w += resultChangeAlpha_5;
	missionStar1Color.w += resultChangeAlpha_5;
	missionStar2Color.w += resultChangeAlpha_5;
	missionStar3Color.w += resultChangeAlpha_5;
	missionStar4Color.w += resultChangeAlpha_9;
	missionStar5Color.w += resultChangeAlpha_10;
	missionStar6Color.w += resultChangeAlpha_11;

	resultTimer -= 1.0f;

	// 値の制御
	if (resultTimer > 0.0f)
	{
		// 値の設定
		resultMoveVelX_1 = 60.0f;
		resultMoveAccX_1 = 1.0f;
		Sound::GetInstance()->PlayWav("SE/Result/result_open_1.wav", seVolume);
	}

	// バナー1の横幅の制御
	if (resultBN_1Size.x > 1280)
	{
		// 値の設定
		resultMoveVelY_1 = 40.0f;
		resultMoveAccY_1 = 1.2f;

		// 値の初期化
		resultBN_1Size.x = 1280;
		resultMoveVelX_1 = 0;
		resultMoveAccX_1 = 0;
	}

	// バナー1の縦幅の制御
	if (resultBN_1Size.y > 624)
	{
		// 値の設定
		resultMoveVelX_2 = 40.0f;
		resultMoveAccX_2 = 1.0f;
		resultMoveVelX_4 = 40.0f;
		resultMoveAccX_4 = 1.0f;
		Sound::GetInstance()->PlayWav("SE/Result/result_open_2.wav", seVolume);

		resultChangeAlpha_1 = 0.05f;

		resultChangeAlpha_6 = 0.05f;
		resultMoveVel_7 = -3.0f;

		// 値の初期化
		resultBN_1Size.y = 624.0f;
		resultMoveVelY_1 = 0.0f;
		resultMoveAccY_1 = 0.0f;
	}

	// RESULTのサイズ制御
	if (resultGTXT_15Size.y < 30.0f)
	{
		// 値の設定
		resultGTXT_15Size.x = 165.0f;
		resultGTXT_15Size.y = 30.0f;
		resultMoveVel_7 = 1.0f;

		resultChangeAlpha_7 = 0.05f;

		changeResultSizeFlag = true;
	}

	if (resultGTXT_15Size.y > 40.0f)
	{
		// 値の初期化
		if (changeResultSizeFlag == true)
		{
			resultGTXT_15Size.x = 220.0f;
			resultGTXT_15Size.y = 40.0f;
			resultMoveVel_7 = 0.0f;
		}
	}

	// RESULTの色制御
	if (resultColor.w > 1.0f)
	{
		// 値の初期化
		resultColor.w = 1.0f;
		resultChangeAlpha_6 = 0.0f;
	}

	// RESULTの背景パーツの色制御
	if (resultPartsColor.w > 1.0f)
	{
		// 値の初期化
		resultPartsColor.w = 1.0f;
		resultChangeAlpha_7 = 0.0f;
	}

	// ラインパーツ1のサイズ制御
	if (resultParts_1Size.x > 646.0f)
	{
		// 値の初期化
		resultParts_1Size.x = 646.0f;
		resultMoveVelX_4 = 0.0f;
		resultMoveAccX_4 = 0.0f;
	}

	// バナー2の横幅の制御 
	if (resultBN_2Size.x > 704.0f)
	{
		// 値の設定
		resultMoveVelX_3 = 40.0f;
		resultMoveAccX_3 = 1.0f;
		//sound->PlayWav("SE/Result/result_open2.wav", masterVolume);

		// 値の初期化
		resultBN_2Size.x = 704.0f;
		resultMoveVelX_2 = 0.0f;
		resultMoveAccX_2 = 0.0f;
	}

	// SCOREの色制御
	if (scoreColor.w > 1.0f)
	{
		// 値の設定
		resultChangeAlpha_2 = 0.05f;

		// 値の初期化
		scoreColor.w = 1.0f;
		resultChangeAlpha_1 = 0.0f;
	}

	// NO DAMAGE BONUSの色制御
	if (noDamageBonusColor.w > 1.0f)
	{
		// 値の設定
		resultChangeAlpha_3 = 0.05f;

		// 値の初期化
		noDamageBonusColor.w = 1.0f;
		resultChangeAlpha_2 = 0.0f;
	}

	// TOTAL SCOREの色制御
	if (totalScoreColor.w > 1.0f)
	{
		// 値の設定
		resultMoveVelX_5 = 20.0f;
		resultMoveAccX_5 = 0.0f;

		// 値の初期化
		totalScoreColor.w = 1.0f;
		resultChangeAlpha_3 = 0.0f;
	}

	// ラインパーツ2のサイズ制御
	if (resultParts_2Size.x > 232.0f)
	{
		// 値の設定
		resultChangeAlpha_4 = 0.05f;

		// 値の初期化
		resultParts_2Size.x = 232.0f;
		resultMoveVelX_5 = 0.0f;
		resultMoveAccX_5 = 0.0f;
	}

	// RANKの色制御
	if (rankColor.w > 1.0f)
	{
		// 値の設定
		resultChangeAlpha_8 = 0.05f;
		resultMoveVel_8 = -6.0f;

		// 値の初期化
		rankColor.w = 1.0f;
		resultChangeAlpha_4 = 0.0f;
	}

	// 各RANKのサイズ制御
	if (rankSSize.x < 80.0f)
	{
		// 値の設定
		resultMoveVelX_6 = 20.0f;
		resultMoveAccX_6 = 0.0f;

		Sound::GetInstance()->PlayWav("SE/Result/result_rank.wav", seVolume);

		// 値の初期化
		rankSSize.x = 80.0f;
		rankSSize.y = 80.0f;

		rankASize.x = 80.0f;
		rankASize.y = 80.0f;

		rankBSize.x = 80.0f;
		rankBSize.y = 80.0f;

		rankCSize.x = 80.0f;
		rankCSize.y = 80.0f;

		resultMoveVel_8 = 0.0f;
	}

	// 各RANKの色制御
	if (rankSColor.w > 1.0f)
	{
		// 値の初期化
		rankSColor.w = 1.0f;
		rankAColor.w = 1.0f;
		rankBColor.w = 1.0f;
		rankCColor.w = 1.0f;
		resultChangeAlpha_8 = 0.0f;
	}

	// ラインパーツ3のサイズ制御
	if (resultParts_3Size.x > 372.0f)
	{
		// 値の設定
		resultChangeAlpha_5 = 0.05f;

		// 値の初期化
		resultParts_3Size.x = 372.0f;
		resultMoveVelX_6 = 0.0f;
		resultMoveAccX_6 = 0.0f;
	}

	// MISSIONの色制御
	if (missionColor.w > 1.0f)
	{
		// 値の設定
		resultMoveVel_9 = -4.0f;
		if (noDamageFlag == true)
		{
			resultChangeAlpha_9 = 0.05f;
		}

		// 値の初期化
		missionColor.w = 1.0f;
		mission1Color.w = 1.0f;
		mission2Color.w = 1.0f;
		mission3Color.w = 1.0f;
		missionStar1Color.w = 1.0f;
		missionStar2Color.w = 1.0f;
		missionStar3Color.w = 1.0f;
		resultChangeAlpha_5 = 0.0f;
	}

	// 星1のサイズ制御
	if (missionStar1Size.x < 28.0f)
	{
		// 値の設定
		resultMoveVel_10 = -4.0f;
		if (allLegBreakFlag == true)
		{
			resultChangeAlpha_10 = 0.05f;
		}
		if (noDamageFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", seVolume);
		}

		// 値の初期化
		missionStar1Size.x = 28.0f;
		missionStar1Size.y = 28.0f;
		resultMoveVel_9 = 0.0f;
	}

	// 星1の色制御
	if (missionStar4Color.w > 1.0f)
	{
		// 値の初期化
		missionStar4Color.w = 1.0f;
		resultChangeAlpha_9 = 0.0f;
	}

	// 星2のサイズ制御
	if (missionStar2Size.x < 28.0f)
	{
		// 値の設定
		resultMoveVel_11 = -4.0f;
		if (targetScoreFlag == true)
		{
			resultChangeAlpha_11 = 0.05f;
		}

		if (allLegBreakFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", seVolume);

		}

		// 値の初期化
		missionStar2Size.x = 28.0f;
		missionStar2Size.y = 28.0f;
		resultMoveVel_10 = 0.0f;
	}

	// 星2の色制御
	if (missionStar5Color.w > 1.0f)
	{
		// 値の初期化
		missionStar5Color.w = 1.0f;
		resultChangeAlpha_10 = 0.0f;
	}

	// 星3のサイズ制御
	if (missionStar3Size.x < 28.0f)
	{
		if (targetScoreFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", seVolume);
		}

		// 値の初期化
		missionStar3Size.x = 28.0f;
		missionStar3Size.y = 28.0f;
		resultMoveVel_11 = 0.0f;
		canPushKeyFlag = true;
	}

	// 星3の色制御
	if (missionStar6Color.w > 1.0f)
	{
		// 値の初期化
		missionStar6Color.w = 1.0f;
		resultChangeAlpha_11 = 0.0f;
	}

	// バナー3の横幅の制御  
	if (resultBN_3Size.x > 704.0f)
	{
		// 値の初期化
		resultBN_3Size.x = 704.0f;
		resultMoveVelX_3 = 0.0f;
		resultMoveAccX_3 = 0.0f;
	}

	// 暗転からシーン遷移
	if (canPushKeyFlag == true)
	{
		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			changeColorFlag = true;
		}
	}

	if (changeColorFlag == true)
	{
		loadingColor.w += 0.05f;
		changeSceneFlag = true;
	}

	if (changeSceneFlag == true)
	{
		changeSceneTimer--;
	}

	if (changeSceneTimer <= 0.0f)
	{
		loadingColor.w = 0.0f;
		TitleInitialize();
		scene = TITLE;
	}


	LoadingBG->SetColor(loadingColor);

	ResultBN_1->SetSize(resultBN_1Size);
	ResultBN_2->SetSize(resultBN_2Size);
	ResultBN_3->SetSize(resultBN_3Size);

	resultParts_1->SetSize(resultParts_1Size);
	resultParts_2->SetSize(resultParts_2Size);
	resultParts_3->SetSize(resultParts_3Size);

	resultParts_7->SetSize(missionStar1Size);
	resultParts_8->SetSize(missionStar2Size);
	resultParts_9->SetSize(missionStar3Size);

	resultGTXT_6->SetSize(rankSSize);
	resultGTXT_7->SetSize(rankASize);
	resultGTXT_8->SetSize(rankBSize);
	resultGTXT_9->SetSize(rankCSize);
	resultGTXT_15->SetSize(resultGTXT_15Size);

	resultParts_4->SetColor(missionStar1Color);
	resultParts_5->SetColor(missionStar2Color);
	resultParts_6->SetColor(missionStar3Color);
	resultParts_7->SetColor(missionStar4Color);
	resultParts_8->SetColor(missionStar5Color);
	resultParts_9->SetColor(missionStar6Color);

	resultParts_10->SetColor(resultPartsColor);

	resultGTXT_1->SetColor(scoreColor);
	resultGTXT_2->SetColor(noDamageBonusColor);
	resultGTXT_3->SetColor(totalScoreColor);
	resultGTXT_4->SetColor(rankColor);
	resultGTXT_5->SetColor(missionColor);
	resultGTXT_6->SetColor(rankSColor);
	resultGTXT_7->SetColor(rankAColor);
	resultGTXT_8->SetColor(rankBColor);
	resultGTXT_9->SetColor(rankCColor);
	resultGTXT_12->SetColor(mission1Color);
	resultGTXT_13->SetColor(mission2Color);
	resultGTXT_14->SetColor(mission3Color);
	resultGTXT_15->SetColor(resultColor);

	camera->Update();

	// デバックテキスト
	AllDebugText();

	// スコアの描画
	std::ostringstream GameScore;
	GameScore << std::fixed << std::setprecision(0) << std::setw(7) << gameScore;
	scoreText.Print(GameScore.str(), scorePosition, { 0.760f, 0.929f, 1.0f, scoreColor.w }, 0.5f);

	std::ostringstream NoDamageBonus;
	NoDamageBonus << std::fixed << std::setprecision(0) << std::setw(7) << noDamageBonus;
	scoreText.Print(NoDamageBonus.str(), noDamageBonusPosition, { 1.0f, 1.0f, 0.4f, noDamageBonusColor.w }, 0.5f);

	std::ostringstream TotalScone;
	TotalScone << std::fixed << std::setprecision(0) << std::setw(7) << totalScore;
	scoreText.Print(TotalScone.str(), totalScorePosition, { 0.760f, 0.929f, 1.0f, totalScoreColor.w }, 0.8f);
}

void GameScene::ResultDraw()
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
	//bossHitParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画
	ResultBN_1->Draw();
	ResultBN_2->Draw();
	ResultBN_3->Draw();

	resultParts_1->Draw();
	resultParts_2->Draw();
	resultParts_3->Draw();
	resultParts_4->Draw();
	resultParts_5->Draw();
	resultParts_6->Draw();
	resultParts_7->Draw();
	resultParts_8->Draw();
	resultParts_9->Draw();
	resultParts_10->Draw();

	resultGTXT_1->Draw();
	resultGTXT_2->Draw();
	resultGTXT_3->Draw();
	resultGTXT_4->Draw();
	resultGTXT_5->Draw();
	resultGTXT_12->Draw();
	resultGTXT_13->Draw();
	resultGTXT_14->Draw();
	resultGTXT_15->Draw();


	if (gameScore > 99999)
	{
		// S
		resultGTXT_6->Draw();
	}
	else if (gameScore > 49999)
	{
		// A
		resultGTXT_7->Draw();
	}
	else if (gameScore > 24999)
	{
		// B
		resultGTXT_8->Draw();
	}
	else
	{
		// C
		resultGTXT_9->Draw();
	}

	// スコアテキストの描画
	scoreText.DrawAll(cmdList);

	LoadingBG->Draw();

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::ResultInitialize()
{
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 0 });
	camera->SetUp({ 0, 1, 0 });

	// LoadingBG->SetColor({ 1, 1, 1, 0 });
	loadingColor.w = 0.0f;

	totalScore = 0.0f;

	noDamageBonus = 20000.0f;

	ResultBN_1->SetSize({ 0.0f, 8.0f });
	ResultBN_1->SetAnchorPoint({ 0.5f, 0.5f });

	ResultBN_2->SetSize({ 0.0f, 160.0f });
	ResultBN_2->SetAnchorPoint({ 0.5f, 0.5f });

	ResultBN_3->SetSize({ 0.0f, 192.0f });
	ResultBN_3->SetAnchorPoint({ 0.5f, 0.5f });

	// Line Parts1
	resultParts_1->SetSize({ 0.0f, 6.0f });
	resultParts_1->SetAnchorPoint({ 0.5f, 0.5f });

	// Line Parts2
	resultParts_2->SetSize({ 0.0f, 6.0f });
	resultParts_2->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_3->SetSize({ 0.0f, 6.0f });
	resultParts_3->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_4->SetSize({ 28.0f, 28.0f });
	resultParts_4->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_4->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_5->SetSize({ 28.0f, 28.0f });
	resultParts_5->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_5->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_6->SetSize({ 28.0f, 28.0f });
	resultParts_6->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_6->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_7->SetSize({ 84.0f, 84.0f });
	resultParts_7->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_7->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_8->SetSize({ 84.0f, 84.0f });
	resultParts_8->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_8->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_9->SetSize({ 84.0f, 84.0f });
	resultParts_9->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_9->SetAnchorPoint({ 0.5f, 0.5f });

	resultParts_10->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultParts_10->SetAnchorPoint({ 0.5f, 0.5f });

	// SCORE
	resultGTXT_1->SetSize({ 76, 16 });
	resultGTXT_1->SetColor({ 0.760f, 0.929f, 1.0f, 0.0f });
	resultGTXT_1->SetAnchorPoint({ 0.0f, 0.5f });

	// NO DAMAGE BONUS
	resultGTXT_2->SetSize({ 213, 16 });
	resultGTXT_2->SetColor({ 1.0f, 1.0f, 0.4f, 0.0f });
	resultGTXT_2->SetAnchorPoint({ 0.0f, 0.5f });

	// TOTAL SCORE
	resultGTXT_3->SetSize({ 158, 16 });
	resultGTXT_3->SetColor({ 0.760f, 0.929f, 1.0f, 0.0f });
	resultGTXT_3->SetAnchorPoint({ 0.0f, 0.5f });

	// RANK
	resultGTXT_4->SetSize({ 61, 16 });
	resultGTXT_4->SetColor({ 0.760f, 0.929f, 1.0f, 0.0f });
	resultGTXT_4->SetAnchorPoint({ 0.0f, 0.5f });

	// MISSION
	resultGTXT_5->SetSize({ 103, 16 });
	resultGTXT_5->SetColor({ 0.760f, 0.929f, 1.0f, 0.0f });
	resultGTXT_5->SetAnchorPoint({ 0.0f, 0.5f });

	// S
	resultGTXT_6->SetSize({ 160.0f, 160.0f });
	resultGTXT_6->SetColor({ 1.0f, 1.0f, 0.0f, 0.0f });
	resultGTXT_6->SetAnchorPoint({ 0.5f, 0.5f });

	// A
	resultGTXT_7->SetSize({ 160.0f, 160.0f });
	resultGTXT_7->SetColor({ 1.0f, 0.4f, 0.0f, 0.0f });
	resultGTXT_7->SetAnchorPoint({ 0.5f, 0.5f });

	// B
	resultGTXT_8->SetSize({ 160.0f, 160.0f });
	resultGTXT_8->SetColor({ 0.0f, 1.0f, 1.0f, 0.0f });
	resultGTXT_8->SetAnchorPoint({ 0.5f, 0.5f });

	// C
	resultGTXT_9->SetSize({ 160.0f, 160.0f });
	resultGTXT_9->SetColor({ 0.0f, 1.0f, 0.0f, 0.0f });
	resultGTXT_9->SetAnchorPoint({ 0.5f, 0.5f });

	// MISSIOM_1
	resultGTXT_12->SetSize({ 217, 20 });
	resultGTXT_12->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultGTXT_12->SetAnchorPoint({ 0.0f, 0.5f });

	// MISSIOM_2
	resultGTXT_13->SetSize({ 209, 20 });
	resultGTXT_13->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultGTXT_13->SetAnchorPoint({ 0.0f, 0.5f });

	// MISSIOM_3
	resultGTXT_14->SetSize({ 198, 20 });
	resultGTXT_14->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultGTXT_14->SetAnchorPoint({ 0.0f, 0.5f });

	// RESULT
	resultGTXT_15->SetSize({ 330.0f, 60.0f });
	resultGTXT_15->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
	resultGTXT_15->SetAnchorPoint({ 0.5f, 0.5f });

	changeColorFlag = false;
	changeColorTimer = 30.0f;

	changeSceneFlag = false;
	changeSceneTimer = 100.0f;

	resultTimer = 5.0f;

	canPushKeyFlag = false;

	changeResultSizeFlag = false;
}

void GameScene::GameOverUpdate()
{

	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		TitleInitialize();
		scene = TITLE;
	}

	camera->Update();

	// デバックテキスト
	AllDebugText();
}

void GameScene::GameOverDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	StageSelectBG->Draw();

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
	//bossHitParticle->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	ObjObject::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 描画

	LoadingBG->Draw();

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScene::GameOverInitialize()
{
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 0 });
	camera->SetUp({ 0, 1, 0 });

	LoadingBG->SetColor({ 1, 1, 1, 0 });

	changeColorFlag = false;
	changeColorTimer = 30.0f;

	changeSceneFlag = false;
	changeSceneTimer = 100.0f;
}

void GameScene::CreateHitParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 0.0f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};

		// 追加
		bossHitParticle->Add(20, pos, vel, acc, { 1.0f,1.0f, 0.0f, 1.0f }, { 1.0f,1.0f, 0.0f, 1.0f }, 3.0f, 0.0f);
	}
}

void GameScene::CreateBossParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 0.1f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 0.01f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.003f;
		acc.y = +(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		bossBreakParticle->Add(30, pos, vel, acc, { 0.874f,0.443f, 0.149f, 1.000f }, { 0.874f,0.443f, 0.149f, 1.000f }, 2.0f, 0.0f);
	}
}

void GameScene::CreatePlayerJetParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.1f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = position.z + -0.8f;

		const float rnd_vel = -0.2f;
		XMFLOAT3 vel{};
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = -0.1f;
		acc.z = -0.1f;

		// 追加
		playerJetParticle->Add(5, pos, vel, acc, { 0.874f,0.443f, 0.149f, 1.000f }, { 0.874f,0.443f, 0.149f, 1.000f }, 0.3f, 0.0f);
	}
}

void GameScene::CreateTitlePlayerJetParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.1f;
		XMFLOAT3 pos{};
		pos.x = position.x + -1.0f;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = -0.4f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = -0.1f;
		acc.x = -0.1f;

		// 追加
		playerJetParticle->Add(5, pos, vel, acc, { 0.874f,0.443f, 0.149f, 1.000f }, { 0.874f,0.443f, 0.149f, 1.000f }, 0.3f, 0.0f);
	}
}

void GameScene::CreatePlayerContrailParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.05f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = position.z;

		const float rnd_vel = -0.2f;
		XMFLOAT3 vel{};
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = -0.1f;
		acc.z = -0.1f;

		// 追加
		playerContrailParticle->Add(5, pos, vel, acc, { 1.0f, 1.0f, 1.0f, 0.3f }, { 1.0f, 1.0f, 1.0f, 0.0f }, 0.1f, 0.0f);
	}
}

void GameScene::CreatePlayerBulletParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		playerBulletParticle->Add(7, pos, vel, acc, { 0.1f,1.0f, 0.1f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.7f, 0.0f);
	}
}

void GameScene::CreatePlayerChargeBulletParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		playerBulletParticle->Add(3, pos, vel, acc, { 0.1f,1.0f, 0.1f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, chargeBulletSize, 0.0f);
	}
}

void GameScene::CreateBossBulletParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.1f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		playerBulletParticle->Add(7, pos, vel, acc, { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, 0.7f, 0.0f);
	}
}

// デバックテキスト
void GameScene::AllDebugText()
{
	std::ostringstream Scene;
	Scene << "Scene:("
		<< std::fixed << std::setprecision(2)
		<< scene << ")";
	debugText.Print(Scene.str(), 50, 10, 1.0f);

	std::ostringstream ResultMoveVel;
	ResultMoveVel << "ResultMoveVel:("
		<< std::fixed << std::setprecision(2)
		<< resultMoveVelY_1 << ")";
	debugText.Print(ResultMoveVel.str(), 50, 30, 1.0f);

	std::ostringstream ResultMoveAcc;
	ResultMoveAcc << "ResultMoveAcc:("
		<< std::fixed << std::setprecision(2)
		<< resultMoveAccY_1 << ")";
	debugText.Print(ResultMoveAcc.str(), 50, 50, 1.0f);
}

void GameScene::TitleDebugText()
{
	std::ostringstream MoveX;
	MoveX << "MoveX:("
		<< std::fixed << std::setprecision(6)
		<< moveX << ")";
	debugText.Print(MoveX.str(), 50, 30, 1.0f);

	std::ostringstream MoveY;
	MoveY << "MoveY:("
		<< std::fixed << std::setprecision(6)
		<< moveY << ")";
	debugText.Print(MoveY.str(), 50, 50, 1.0f);

	std::ostringstream titlePlayerPos;
	titlePlayerPos << "TitlePlayerPos:("
		<< std::fixed << std::setprecision(2)
		<< TitlePlayerPosition.x << "," // x
		<< TitlePlayerPosition.y << "," // y
		<< TitlePlayerPosition.z << ") Local"; // z
	debugText.Print(titlePlayerPos.str(), 50, 70, 1.0f);
}

void GameScene::GameDebugText()
{
	// プレイヤーの座標を表示
	std::ostringstream PlayerPos;
	PlayerPos << "PlayerPos:("
		<< std::fixed << std::setprecision(2)
		<< playerRotation.x << "," // x
		<< playerRotation.y << "," // y
		<< playerRotation.z << ") Local"; // z
	debugText.Print(PlayerPos.str(), 50, 70, 1.0f);

	//std::ostringstream HPUIPos;
	//HPUIPos << "playerHpUIPosition:("
	//	<< std::fixed << std::setprecision(2)
	//	<< playerHpUIPosition.x << "," // x
	//	<< playerHpUIPosition.y << ")";// y
	//debugText.Print(HPUIPos.str(), 50, 90, 1.0f);

	//std::ostringstream BOSSUIPos;
	//BOSSUIPos << "bossHpUIPosition:("
	//	<< std::fixed << std::setprecision(2)
	//	<< bossHpUIPosition.x << "," // x
	//	<< bossHpUIPosition.y << ")";// y
	//debugText.Print(BOSSUIPos.str(), 50, 110, 1.0f);

	//std::ostringstream SCOREUIPos;
	//SCOREUIPos << "SCOREUIPosition:("
	//	<< std::fixed << std::setprecision(2)
	//	<< scoreUIPosition.x << "," // x
	//	<< scoreUIPosition.y << ")";// y
	//debugText.Print(SCOREUIPos.str(), 50, 130, 1.0f);

	std::ostringstream bossPos;
	bossPos << "BossPos:("
		<< std::fixed << std::setprecision(2)
		<< bossWorldPosition.x << "," // x
		<< bossRotation.y << "," // y
		<< bossWorldPosition.z << ")"; // z
	debugText.Print(bossPos.str(), 50, 350, 1.0f);

	//std::ostringstream CenterPos;
	//CenterPos << "CenterPos:("
	//	<< std::fixed << std::setprecision(2)
	//	<< centerPosition.x << "," // x
	//	<< centerPosition.y << "," // y
	//	<< centerPosition.z << ") World"; // z
	//debugText.Print(CenterPos.str(), 50, 110, 1.0f);

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
	debugText.Print(TimeRate.str(), 50, 190, 1.0f);*/

	std::ostringstream StartIndex;
	StartIndex << "StartIndex:("
		<< std::fixed << std::setprecision(2)
		<< playerChargeFlag << ")";
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);

	// ボスのHP関連
	std::ostringstream BossHp;
	BossHp << "playerChargeNow:("
		<< std::fixed << std::setprecision(2)
		<< playerChargeNow << ")";
	debugText.Print(BossHp.str(), 50, 230, 1.0f);

	std::ostringstream BossLeg1Hp;
	BossLeg1Hp << "BossLeg1Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg1Hp << ")";
	debugText.Print(BossLeg1Hp.str(), 50, 250, 1.0f);

	std::ostringstream BossLeg2Hp;
	BossLeg2Hp << "BossLeg2Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg2Hp << ")";
	debugText.Print(BossLeg2Hp.str(), 50, 270, 1.0f);

	std::ostringstream BossLeg3Hp;
	BossLeg3Hp << "BossLeg3Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg3Hp << ")";
	debugText.Print(BossLeg3Hp.str(), 50, 290, 1.0f);

	std::ostringstream BossLeg4Hp;
	BossLeg4Hp << "BossLeg4Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg4Hp << ")";
	debugText.Print(BossLeg4Hp.str(), 50, 310, 1.0f);

	std::ostringstream PlayerHp;
	PlayerHp << "PlayerHp:("
		<< std::fixed << std::setprecision(2)
		<< playerHpRatio << ")";
	debugText.Print(PlayerHp.str(), 50, 330, 1.0f);

	// プレイヤーの回避関連
	/*std::ostringstream DodgeRollFlag;
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
	debugText.Print(DodgeRollRotation.str(), 50, 290, 1.0f);*/
}

// カメラ方向の切り替え
void GameScene::CameraSwitching()
{
	switch (cameraType)
	{
	case CAMERAMODE::START:
		L5nowCount = 0.0f;
		L5addCount = 0.02f;
		cameraType = FIGHT;

		break;

	case CAMERAMODE::FIGHT:
		if (moveCameraNumber == 0)
		{
			objCamera->SetPosition(moveCameraPosition_1);
		}
		if (moveCameraNumber == 1)
		{
			objCamera->SetPosition(moveCameraPosition_2);
		}
		if (moveCameraNumber == 2)
		{
			objCamera->SetPosition(moveCameraPosition_3);
		}
		if (moveCameraNumber >= 3)
		{
			objCamera->SetPosition(cameraLocalPosition);
		}
		break;
	}

	camera->SetEye(cameraWorldPosition);

	// 前方向
	if (cameraMode == 0)
	{
		playerLocalPosition.z = 0;

		nextCamera = { 0.0f,0.0f,-20.0f };
	}

	// 右方向
	if (cameraMode == 1)
	{
		playerLocalPosition.x = 0;

		nextCamera = { -20.0f,0.0f,0.0f };
	}

	// 後方向
	if (cameraMode == 2)
	{
		playerLocalPosition.z = 0;

		nextCamera = { 0.0f,0.0f,20.0f };
	}

	// 左方向
	if (cameraMode == 3)
	{
		playerLocalPosition.x = 0;

		nextCamera = { 20.0f,0.0f,0.0f };
	}

	if (startIndex == 3)
	{
		L4addCount = moveUIVel;
	}

	if (startIndex == 4)
	{
		playerUpdateFlag = true;
	}

	if (startIndex == 12)
	{
		cameraMode = 2;
		backFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;
	}

	else if (startIndex == 29)
	{
		cameraMode = 0;
		backFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;

	}

	else if (startIndex == 41)
	{
		cameraMode = 1;
		rightFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;

	}

	else if (startIndex == 49)
	{
		cameraMode = 3;
		backFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;

	}

	else if (startIndex == 57)
	{
		cameraMode = 0;
		rightFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;

	}

	/*if (Input::GetInstance()->TriggerKey(DIK_RIGHT))
	{
		cameraMode = 3;
		rightFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_DOWN))
	{
		cameraMode = 0;
		backFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_LEFT))
	{
		cameraMode = 1;
		leftFlashingFlag = true;
		L3nowCount = 0.0f;
		L3addCount = 0.001f;
	}*/
}

void GameScene::Attack()
{
	shotRate -= 0.1f;

	XMVECTOR bulletVelocity = { 0,0,playerBulletSpeed };

	if (playerUpdateFlag == true)
	{
		if (Input::GetInstance()->PushKey(DIK_SPACE) || (IsButtonPush(ButtonKind::Button_A)))
		{
			if (shotRate <= 0)
			{
				shotFlag = true;
			}

			if (shotFlag == true)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
				newBullet = Bullet::Create(modelBullet, playerWorldPosition, bulletScale, playerBulletSpeed);

				bullets.push_back(std::move(newBullet));

				shotFlag = false;
				shotRate = 1.5f;
			}
		}
	}
}

void GameScene::chargeAttack()
{
	if (playerUpdateFlag == true)
	{
		playerChargeRatio = playerChargeNow / playerChargeMax;
		chargeBulletSize = Easing::InOutQuadFloat(0.0, 2.0, playerChargeRatio);

		CreatePlayerChargeBulletParticles(playerWorldPosition);

		if (playerChargeFlag == false)
		{
			if (playerChargeNow < playerChargeMax)
			{
				if (Input::GetInstance()->PushKey(DIK_SPACE))
				{
					playerChargeNow++;
				}
				else if (!Input::GetInstance()->PushKey(DIK_SPACE))
				{
					playerChargeNow--;
				}
			}
			else
			{
				if (!Input::GetInstance()->PushKey(DIK_SPACE))
				{
					playerChargeFlag = true;
				}
			}
		}
		else
		{
			Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet = Bullet::Create(modelBullet, playerWorldPosition, bulletScale, playerBulletSpeed);

			bullets.push_back(std::move(newBullet));

			playerChargeNow = 0.0f;
			playerChargeFlag = false;
		}
		playerChargeNow = max(playerChargeNow, 0.0);
		playerChargeNow = min(playerChargeNow, playerChargeMax);
	}
}

void GameScene::BossAttack()
{
	/*std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(modelBullet, bossLocalPosition, bulletScale, bulletVelocity);

	bossBullets.push_back(std::move(newBullet));*/

	if (playerHp >= 0.0f)
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossWorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScene::BossLeg1Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg1Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg1WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScene::BossLeg2Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg2Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg2WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScene::BossLeg3Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg3Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg3WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScene::BossLeg4Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg4Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg4WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScene::SplineCount()
{
	nowCount = GetTickCount();

	elapsedCount = nowCount - startCount;
	elapsedTime = static_cast<float> (elapsedCount) / 1000.0f;

	timeRate = elapsedCount / maxTime;

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
}

void GameScene::Lerp1Count()
{
	L1nowCount = GetTickCount();

	L1elapsedCount = L1nowCount - L1startCount;
	L1elapsedTime = static_cast<float> (L1elapsedCount) / 1000.0f;

	L1timeRate = min(L1elapsedCount / L1maxTime, 1.0f);
}

void GameScene::Lerp2Count()
{
	L2nowCount = GetTickCount();

	L2elapsedCount = L2nowCount - L2startCount;
	L2elapsedTime = static_cast<float> (L2elapsedCount) / 1000.0f;

	L2timeRate = min(L2elapsedCount / L2maxTime, 1.0f);
}

void GameScene::Lerp3Count()
{
	L3nowCount += L3addCount;

	if (L3nowCount > 0.1f)
	{
		playerUpdateFlag = true;
		L3nowCount = 0.1f;
		L3addCount = 0.0f;
	}

	if (L3nowCount < 0.1f)
	{
		playerUpdateFlag = false;
	}
}

void GameScene::Lerp4Count()
{
	L4nowCount += L4addCount;

	if (L4nowCount > 1.0f)
	{
		L4nowCount = 1.0f;
		L4addCount = 0.0f;
	}

	if (L4nowCount < 0.0f)
	{
		L4nowCount = 0.0f;
		L4addCount = 0.0f;
	}
}

void GameScene::Lerp5Count()
{
	L5nowCount += L5addCount;
	moveCameraTimer += moveCameraTimerVel;

	if (L5nowCount > 1.0f)
	{
		L5addCount = 0.0f;
		moveCameraTimerVel = -0.1f;
		if (moveCameraTimer < 0.0f)
		{
			moveCameraTimer = 2.0f;
			moveCameraTimerVel = 0.0f;
			moveCameraNumber++;
			L5nowCount = 0.0f;
			L5addCount = 0.02f;
		}
	}

	if (moveCameraNumber > 4)
	{
		L5nowCount = 1.0f;
		L5addCount = 0.0f;
	}
}

void GameScene::RankTimer()
{
	//タイマーを更新
	const float rankTime = 40;
	rankTimer++;
	const float rankTimeRate = rankTimer / rankTime;


	if (gameScore > 99999)
	{
		// S
		if (rankSOneTimeFlag)
		{
			rankTimer = 0;
			rankSOneTimeFlag = false;
		}
	}
	else if (gameScore > 49999)
	{
		// A
		if (rankAOneTimeFlag)
		{
			rankTimer = 0;
			rankAOneTimeFlag = false;
		}
	}
	else if (gameScore > 24999)
	{
		// B
		if (rankBOneTimeFlag)
		{
			rankTimer = 0;
			rankBOneTimeFlag = false;
		}
	}
	else
	{
		// C
		if (rankCOneTimeFlag)
		{
			rankTimer = 0;
			rankCOneTimeFlag = false;
		}
	}

	gameGTXT_1Size = Easing::OutCubicFloat2({ 120.0f, 120.0f }, { 60.0f, 60.0f }, rankTimeRate);
	gameGTXT_1Color.w = Easing::OutCubicFloat(1.0f, 0.0f, rankTimeRate);

	gameGTXT_2Size = Easing::OutCubicFloat2({ 120.0f, 120.0f }, { 60.0f, 60.0f }, rankTimeRate);
	gameGTXT_2Color.w = Easing::OutCubicFloat(1.0f, 0.0f, rankTimeRate);

	gameGTXT_3Size = Easing::OutCubicFloat2({ 120.0f, 120.0f }, { 60.0f, 60.0f }, rankTimeRate);
	gameGTXT_3Color.w = Easing::OutCubicFloat(1.0f, 0.0f, rankTimeRate);

	gameGTXT_4Size = Easing::OutCubicFloat2({ 120.0f, 120.0f }, { 60.0f, 60.0f }, rankTimeRate);
	gameGTXT_4Color.w = Easing::OutCubicFloat(1.0f, 0.0f, rankTimeRate);

	if (rankTimer >= rankTime)
	{
		rankTimer = rankTime;
	}
}

void GameScene::ScoreRateTimer()
{
	const float scoreRateTime = 40;
	scoreRateTimer++;
	const float scoreRateTimeRate = scoreRateTimer / scoreRateTime;

	if (scoreRateCount >= 50)
	{
		if (rate3rdOneTimeFlag)
		{
			scoreRateTimer = 0;
			rate1stOneTimeFlag = true;
			rate2ndOneTimeFlag = true;
			rate3rdOneTimeFlag = false;
		}

	}
	else if (scoreRateCount >= 20)
	{
		if (rate2ndOneTimeFlag)
		{
			scoreRateTimer = 0;
			rate1stOneTimeFlag = true;
			rate3rdOneTimeFlag = true;
			rate2ndOneTimeFlag = false;
		}
	}
	else
	{
		if (rate1stOneTimeFlag)
		{
			scoreRateTimer = 0;
			rate2ndOneTimeFlag = true;
			rate3rdOneTimeFlag = true;
			rate1stOneTimeFlag = false;
		}
	}

	scoreRateAlpha = Easing::OutCubicFloat(1.0f, 0.0f, scoreRateTimeRate);

	if (scoreRateTimer >= scoreRateTime)
	{
		scoreRateTimer = scoreRateTime;
	}
}

void GameScene::LoadTextureFunction()
{
	// 共通

	// タイトル
	if (!Sprite::LoadTexture(TextureNumber::title_gtxt_1, L"Resources/Sprite/TitleUI/title_gtxt_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::title_parts_1, L"Resources/Sprite/TitleUI/title_parts_1.png")) {
		assert(0);
		return;
	}

	// ゲーム
	if (!Sprite::LoadTexture(TextureNumber::game_parts_1, L"Resources/Sprite/GameUI/game_parts_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_number1, L"Resources/Sprite/GameUI/game_gtxt_number1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_number2, L"Resources/Sprite/GameUI/game_gtxt_number2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_number3, L"Resources/Sprite/GameUI/game_gtxt_number3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_gtxt_GO, L"Resources/Sprite/GameUI/game_gtxt_GO.png")) {
		assert(0);
		return;
	}

	// プレイヤー
	if (!Sprite::LoadTexture(TextureNumber::game_player_frame_1, L"Resources/Sprite/GameUI/PlayerHpUI/game_player_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_player_frame_2, L"Resources/Sprite/GameUI/PlayerHpUI/game_player_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_player_gauge, L"Resources/Sprite/GameUI/PlayerHpUI/game_player_gauge.png")) {
		assert(0);
		return;
	}

	// ボス
	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_1, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_frame_2, L"Resources/Sprite/GameUI/BossHpUI/game_boss_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_boss_gauge, L"Resources/Sprite/GameUI/BossHpUI/game_boss_gauge.png")) {
		assert(0);
		return;
	}

	// スコア
	if (!Sprite::LoadTexture(TextureNumber::game_score_frame, L"Resources/Sprite/GameUI/ScoreUI/game_score_frame.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_score_gtxt, L"Resources/Sprite/GameUI/ScoreUI/game_score_gtxt.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_score_gtxt_2, L"Resources/Sprite/GameUI/ScoreUI/game_score_gtxt_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::game_score_parts, L"Resources/Sprite/GameUI/ScoreUI/game_score_parts.png")) {
		assert(0);
		return;
	}

	// エフェクト
	if (!Sprite::LoadTexture(TextureNumber::loading_effect_1, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::damage_effect_1, L"Resources/Sprite/Effect/damage_effect_1.png")) {
		assert(0);
		return;
	}

	// リザルト
	if (!Sprite::LoadTexture(TextureNumber::result_bg, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_frame_1, L"Resources/Sprite/ResultUI/result_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_frame_2, L"Resources/Sprite/ResultUI/result_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_frame_3, L"Resources/Sprite/ResultUI/result_frame_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_1, L"Resources/Sprite/ResultUI/result_gtxt_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_2, L"Resources/Sprite/ResultUI/result_gtxt_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_3, L"Resources/Sprite/ResultUI/result_gtxt_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_4, L"Resources/Sprite/ResultUI/result_gtxt_4.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_5, L"Resources/Sprite/ResultUI/result_gtxt_5.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_6, L"Resources/Sprite/ResultUI/result_gtxt_6.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_7, L"Resources/Sprite/ResultUI/result_gtxt_7.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_8, L"Resources/Sprite/ResultUI/result_gtxt_8.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_9, L"Resources/Sprite/ResultUI/result_gtxt_9.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_12, L"Resources/Sprite/ResultUI/result_gtxt_12.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_13, L"Resources/Sprite/ResultUI/result_gtxt_13.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_14, L"Resources/Sprite/ResultUI/result_gtxt_14.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_gtxt_15, L"Resources/Sprite/ResultUI/result_gtxt_15.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_1, L"Resources/Sprite/ResultUI/result_parts_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_2, L"Resources/Sprite/ResultUI/result_parts_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_3, L"Resources/Sprite/ResultUI/result_parts_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_4, L"Resources/Sprite/ResultUI/result_parts_4.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_5, L"Resources/Sprite/ResultUI/result_parts_5.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::result_parts_6, L"Resources/Sprite/ResultUI/result_parts_6.png")) {
		assert(0);
		return;
	}
}

void GameScene::LoadWavFunction()
{
	Sound::GetInstance()->LoadWav("BGM/Title/title_bgm.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_player_shot.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_boss_shot.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_player_damage.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_boss_damage.wav");
	Sound::GetInstance()->LoadWav("BGM/Game/game_bgm.wav");
	Sound::GetInstance()->LoadWav("SE/Title/title_start.wav");
	Sound::GetInstance()->LoadWav("SE/Result/result_open_1.wav");
	Sound::GetInstance()->LoadWav("SE/Result/result_open_2.wav");
	Sound::GetInstance()->LoadWav("SE/Result/result_mission.wav");
	Sound::GetInstance()->LoadWav("SE/Result/result_rank.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_alert.wav");
}

void GameScene::scoreUIMotion()
{
	scoreMoveVel = -8;
}

void GameScene::scoreUIUpdate()
{
	scoreMoveVel += scoreMoveAcc;
	scoreBasePosition.y += scoreMoveVel;

	if (scoreBasePosition.y < -8.0f)
	{
		scoreMoveVel = 0;
		scoreBasePosition.y = -8.0f;
	}

	if (scoreBasePosition.y > 0.0f)
	{
		scoreMoveVel = 0;
		scoreBasePosition.y = 0.0f;
	}
}

// プレイヤーとUIが重なった際のアルファ値計算
void GameScene::changeGameUIAlpha()
{
	if (cameraMode == 0)
	{
		if (playerLocalPosition.x < -3.2f && playerLocalPosition.y > 6.0f)
		{
			scoreUIAlphaVel = -0.06f;
		}
		else
		{
			scoreUIAlphaVel = 0.06f;
		}

		if (playerLocalPosition.x > 3.6f && playerLocalPosition.y < -8.8f)
		{
			playerHpUIAlphaVel = -0.06f;
			playerDamageUIAlphaVel = -0.08f;
		}
		else
		{
			playerHpUIAlphaVel = 0.06f;
			playerDamageUIAlphaVel = 0.08f;
		}

		if (playerLocalPosition.x > 0.4f && playerLocalPosition.y > 8.8f)
		{
			bossHpUIAlphaVel = -0.06f;
			bossDamageUIAlphaVel = -0.08f;
		}
		else
		{
			bossHpUIAlphaVel = 0.06f;
			bossDamageUIAlphaVel = 0.08f;
		}
	}

	if (cameraMode == 2)
	{
		if (playerLocalPosition.x > 3.2f && playerLocalPosition.y > 6.0f)
		{
			scoreUIAlphaVel = -0.06f;
		}
		else
		{
			scoreUIAlphaVel = 0.06f;
		}

		if (playerLocalPosition.x < -3.6f && playerLocalPosition.y < -8.8f)
		{
			playerHpUIAlphaVel = -0.06f;
			playerDamageUIAlphaVel = -0.08f;
		}
		else
		{
			playerHpUIAlphaVel = 0.06f;
			playerDamageUIAlphaVel = 0.08f;
		}

		if (playerLocalPosition.x < -0.4f && playerLocalPosition.y > 8.8f)
		{
			bossHpUIAlphaVel = -0.06f;
			bossDamageUIAlphaVel = -0.08f;
		}
		else
		{
			bossHpUIAlphaVel = 0.06f;
			bossDamageUIAlphaVel = 0.08f;
		}
	}

	// スコアUIのAlpha値計算
	scoreUIAlpha += scoreUIAlphaVel;

	if (scoreUIAlpha < 0.3f)
	{
		scoreUIAlphaVel = 0.0f;
		scoreUIAlpha = 0.3f;
	}

	if (scoreUIAlpha > 1.0f)
	{
		scoreUIAlphaVel = 0.0f;
		scoreUIAlpha = 1.0f;
	}

	// プレイヤーHPUIのAlpha値計算
	playerHpUIAlpha += playerHpUIAlphaVel;

	if (playerHpUIAlpha < 0.3f)
	{
		playerHpUIAlphaVel = 0.0f;
		playerHpUIAlpha = 0.3f;
	}

	if (playerHpUIAlpha > 1.0f)
	{
		playerHpUIAlphaVel = 0.0f;
		playerHpUIAlpha = 1.0f;
	}

	playerDamageUIAlpha += playerDamageUIAlphaVel;

	if (playerDamageUIAlpha < 0.0f)
	{
		playerDamageUIAlphaVel = 0.0f;
		playerDamageUIAlpha = 0.0f;
	}

	if (playerDamageUIAlpha > 1.0f)
	{
		playerDamageUIAlphaVel = 0.0f;
		playerDamageUIAlpha = 1.0f;
	}

	// ボスHPUIのAlpha値計算
	bossHpUIAlpha += bossHpUIAlphaVel;

	if (bossHpUIAlpha < 0.3f)
	{
		bossHpUIAlphaVel = 0.0f;
		bossHpUIAlpha = 0.3f;
	}

	if (bossHpUIAlpha > 1.0f)
	{
		bossHpUIAlphaVel = 0.0f;
		bossHpUIAlpha = 1.0f;
	}

	bossDamageUIAlpha += bossDamageUIAlphaVel;

	if (bossDamageUIAlpha < 0.0f)
	{
		bossDamageUIAlphaVel = 0.0f;
		bossDamageUIAlpha = 0.0f;
	}

	if (bossDamageUIAlpha > 1.0f)
	{
		bossDamageUIAlphaVel = 0.0f;
		bossDamageUIAlpha = 1.0f;
	}
}

// ダメージエフェクトの挙動管理
void GameScene::damageEffectUpdate()
{
	// ダメージエフェクトの計算
	damageEffectAlpha += damageEffectAlphaVel;

	if (damageEffectAlpha < 0.0f)
	{
		damageEffectAlphaVel = 0.0f;
		damageEffectAlpha = 0.0f;
	}
}

// カメラ切り替え時のUI挙動管理
void GameScene::alertUIUpdate()
{
	// UIの点滅
	if (backFlashingFlag == true)
	{
		gameParts1Color.w -= 0.02f;

		if (backFlashingCount != 3)
		{
			if (gameParts1Color.w <= 0.3f)
			{
				backFlashingCount += 1;
				Sound::GetInstance()->PlayWav("SE/Game/game_alert.wav", 0.3);
				gameParts1Color.w = 1.0f;
			}
		}
		else if (backFlashingCount == 3)
		{
			if (gameParts1Color.w <= 0.0f)
			{
				backFlashingFlag = false;
				backFlashingCount = 0;
				gameParts1Color.w = 0.0f;
			}
		}
	}

	// UIの点滅
	if (rightFlashingFlag == true)
	{
		gameParts2Color.w -= 0.02f;

		if (rightFlashingCount != 3)
		{
			if (gameParts2Color.w <= 0.3f)
			{
				rightFlashingCount += 1;
				Sound::GetInstance()->PlayWav("SE/Game/game_alert.wav", 0.3);
				gameParts2Color.w = 1.0f;
			}
		}
		else if (rightFlashingCount == 3)
		{
			if (gameParts2Color.w <= 0.0f)
			{
				rightFlashingFlag = false;
				rightFlashingCount = 0;
				gameParts2Color.w = 0.0f;
			}
		}
	}

	// UIの点滅
	if (leftFlashingFlag == true)
	{
		gameParts3Color.w -= 0.02f;

		if (leftFlashingCount != 3)
		{
			if (gameParts3Color.w <= 0.3f)
			{
				leftFlashingCount += 1;
				Sound::GetInstance()->PlayWav("SE/Game/game_alert.wav", 0.3);
				gameParts3Color.w = 1.0f;
			}
		}
		else if (leftFlashingCount == 3)
		{
			if (gameParts3Color.w <= 0.0f)
			{
				leftFlashingFlag = false;
				leftFlashingCount = 0;
				gameParts3Color.w = 0.0f;
			}
		}
	}
}

// レールオブジェクトの角度計算
void GameScene::railTargetCalc()
{
	// レールの進行方向へ自機の角度を向ける
	testX = playerTargetPosition.x - centerPosition.x;
	testZ = playerTargetPosition.z - centerPosition.z;
	testRadians = atan2(testZ, testX);
	testDegrees = XMConvertToDegrees(testRadians);
}

// プレイヤーHPの計算
void GameScene::playerHpCalc()
{
	// プレイヤーのHP計算
	playerHpRatio = playerHp / playerHpMax;
	playerHpGageSize.x = playerHpRatio * 320.0f;
}

// ボスHPの計算
void GameScene::bossHpCalc()
{
	// ボスのHP計算
	bossHpRatio = bossHp / bossHpMax;
	bossHpGageSize.x = bossHpRatio * 530.0f;
}

// 各音量の計算
void GameScene::VolumeCalc()
{
	//　マスター音量 = 現在のマスター音量 / 100
	masterVolume = masterVolumeNow / 100.0f;

	//　割合 = 現在のbgm音量 / bgm音量の最大
	bgmVolumeRatio = bgmVolumeNow / bgmVolumeMax;
	//　bgm音量 = (割合 * マスター音量) / 100
	bgmVolume = (bgmVolumeRatio * masterVolumeNow) / 100.0f;

	//　割合 = 現在のse音量 / se音量の最大
	seVolumeRatio = seVolumeNow / seVolumeMax;
	//　se音量 = (割合 * マスター音量) / 100
	seVolume = (seVolumeRatio * masterVolumeNow) / 100.0f;
}

bool GameScene::OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB)
{
	float Check = sqrtf((sphereA.x - sphereB.x) * (sphereA.x - sphereB.x) + (sphereA.y - sphereB.y) * (sphereA.y - sphereB.y) + (sphereA.z - sphereB.z) * (sphereA.z - sphereB.z));
	if (Check <= radiusA - radiusB || Check <= radiusB - radiusA || Check < radiusA + radiusB)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GameScene::CreateCloud()
{
	XMFLOAT3 randPos;

	randPos.x = ((float)rand() / RAND_MAX * 128.0f - 128.0f / 2.0f);
	randPos.y = ((float)rand() / RAND_MAX * 72.0f - 72.0f / 2.0f);
	randPos.z = centerPosition.z + 100.0f;

	if (Input::GetInstance()->TriggerKey(DIK_U))
	{
		std::unique_ptr<StageObject> newCloud = std::make_unique<StageObject>();
		newCloud = StageObject::Create(modelCloud_1, randPos, { 1.0f,1.0f,1.0f }, 0.0f);

		stageObjects.push_back(std::move(newCloud));
	}
}