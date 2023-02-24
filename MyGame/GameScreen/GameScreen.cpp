#include "GameScreen.h"

extern int cameraMode = 0;

using namespace DirectX;

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
	safe_delete(spriteBG);
	safe_delete(objSkydome);
	safe_delete(objGround);
	safe_delete(player);
	safe_delete(modelSkydome);
	safe_delete(modelGround);
	safe_delete(modelPlayer);
	safe_delete(modelBullet);

	safe_delete(testmodel);
	safe_delete(testobject);

	sound->Finalize();
}

void GameScreen::Initialize(DirectXCommon* dxCommon, Input* input, Sound* sound)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(sound);

	this->dxCommon = dxCommon;
	this->input = input;
	this->sound = sound;

	//コントローラー初期化
	InitInput();

	// サウンド初期化
	sound->Initialize();

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

	// スコアテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(scoreTextTexNumber, L"Resources/scorefont.png")) {
		assert(0);
		return;
	}
	// スコアテキスト初期化
	scoreText.Initialize(scoreTextTexNumber);

	//音声の読み込み
	LoadWavFunction();

	// テクスチャ読み込みw
	LoadTextureFunction();

	// スプライト生成
	TitleBG = Sprite::Create(1, { 0.0f,0.0f });

	TitleLogo = Sprite::Create(1, { 100.0f,100.0f }, { 1, 1, 1, 1 });
	LoadingBG = Sprite::Create(3, { 0.0f,0.0f }, { 1,1,1,0 });
	StageSelectBG = Sprite::Create(4, { 0.0f,0.0f });

	GameFG = Sprite::Create(26, { 0.0f,0.0f });
	TitleStartUI = Sprite::Create(27, { 310.0f,630.0f });

	playerHpUI = Sprite::Create(5, { 1164.0f, playerHpUIHigh });
	playerHpGage = Sprite::Create(6, { 1154.0f, playerHpUIHigh });
	playerDamageGage = Sprite::Create(6, { 1154.0f, playerHpUIHigh });
	playerHpUICover = Sprite::Create(9, { 1164.0f, playerHpUIHigh });

	bossHpUI = Sprite::Create(7, { 1265.0f, bossHpUIHigh });
	bossHpGage = Sprite::Create(8, { 1255.0f, bossHpUIHigh });
	bossDamageGage = Sprite::Create(8, { 1255.0f, bossHpUIHigh });
	bossHpUICover = Sprite::Create(38, { 1265.0f, bossHpUIHigh });

	scoreUI = Sprite::Create(11, { 15.0f, bossHpUIHigh + 30.0f });
	scoreChar = Sprite::Create(12, { 190.0f, 30.0f });
	scoreNull_1 = Sprite::Create(13, { 403.5f, 27.0f });
	scoreNull_2 = Sprite::Create(13, { 378.0f, 27.0f });
	scoreNull_3 = Sprite::Create(13, { 352.5f, 27.0f });
	scoreNull_4 = Sprite::Create(13, { 326.5f, 27.0f });
	scoreNull_5 = Sprite::Create(13, { 300.5f, 27.0f });
	scoreNull_6 = Sprite::Create(13, { 275.0f, 27.0f });

	gameGTXT_1 = Sprite::Create(22, { 50.0f,50.0f });
	gameGTXT_2 = Sprite::Create(23, { 50.0f,50.0f });
	gameGTXT_3 = Sprite::Create(24, { 50.0f,50.0f });
	gameGTXT_4 = Sprite::Create(25, { 50.0f,50.0f });

	ResultBG = Sprite::Create(2, { 0.0f,0.0f });
	ResultBN_1 = Sprite::Create(14, { 640.0f,360.0f });
	ResultBN_2 = Sprite::Create(15, { 640.0f,320.0f });
	ResultBN_3 = Sprite::Create(16, { 640.0f,520.0f });

	resultGTXT_1 = Sprite::Create(17, { 330.0f,270.0f });
	resultGTXT_2 = Sprite::Create(18, { 330.0f,300.0f });
	resultGTXT_3 = Sprite::Create(19, { 330.0f,340.0f });
	resultGTXT_4 = Sprite::Create(20, { 330.0f,455.0f });
	resultGTXT_5 = Sprite::Create(21, { 600.0f,455.0f });
	resultGTXT_6 = Sprite::Create(22, { 500.0f,550.0f });
	resultGTXT_7 = Sprite::Create(23, { 500.0f,550.0f });
	resultGTXT_8 = Sprite::Create(24, { 500.0f,550.0f });
	resultGTXT_9 = Sprite::Create(25, { 500.0f,550.0f });
	resultGTXT_12 = Sprite::Create(33, { 600.0f,510.0f });
	resultGTXT_13 = Sprite::Create(34, { 600.0f,545.0f });
	resultGTXT_14 = Sprite::Create(35, { 600.0f,580.0f });
	resultGTXT_15 = Sprite::Create(36, { 640.0f,120.0f });

	resultParts_1 = Sprite::Create(28, { 640.0f,320.0f });
	resultParts_2 = Sprite::Create(29, { 435.0f,480.0f });
	resultParts_3 = Sprite::Create(30, { 775.0f,480.0f });
	resultParts_4 = Sprite::Create(31, { 930.0f,510.0f });
	resultParts_5 = Sprite::Create(31, { 930.0f,545.0f });
	resultParts_6 = Sprite::Create(31, { 930.0f,580.0f });
	resultParts_7 = Sprite::Create(32, { 930.0f,510.0f });
	resultParts_8 = Sprite::Create(32, { 930.0f,545.0f });
	resultParts_9 = Sprite::Create(32, { 930.0f,580.0f });
	resultParts_10 = Sprite::Create(37, { 640.0f,120.0f });

	damageEffect = Sprite::Create(39, { 0.0f, 0.0 });

	// パーティクルマネージャー
	particleMan = ParticleManager::Create(dxCommon->GetDevice(), camera);

	// 3Dオブジェクト生成
	objSkydome = ObjObject::Create();
	objGround = ObjObject::Create();

	objStage1 = ObjObject::Create();
	objStage2 = ObjObject::Create();
	objStage3 = ObjObject::Create();

	player = Player::Create();

	objCenter = ObjObject::Create();

	frontCamera = ObjObject::Create();
	rightCamera = ObjObject::Create();
	backCamera = ObjObject::Create();
	leftCamera = ObjObject::Create();

	bossBody = Boss::Create();
	bossLeg1 = Boss::Create();
	bossLeg2 = Boss::Create();
	bossLeg3 = Boss::Create();
	bossLeg4 = Boss::Create();

	objTitlePlayer = ObjObject::Create();

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");
	modelBossLeg = ObjModel::CreateFromOBJ("BossLeg");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	player->SetModel(modelPlayer);

	objCenter->SetModel(modelBullet);
	frontCamera->SetModel(modelBullet);
	rightCamera->SetModel(modelBullet);
	backCamera->SetModel(modelBullet);
	leftCamera->SetModel(modelBullet);

	objStage1->SetModel(modelBullet);
	objStage2->SetModel(modelBullet);
	objStage3->SetModel(modelBullet);

	bossBody->SetModel(modelBullet);
	bossLeg1->SetModel(modelBossLeg);
	bossLeg2->SetModel(modelBossLeg);
	bossLeg3->SetModel(modelBossLeg);
	bossLeg4->SetModel(modelBossLeg);

	objTitlePlayer->SetModel(modelPlayer);

	// 親子関係を結ぶ
	player->SetParent(objCenter);

	frontCamera->SetParent(objCenter);
	rightCamera->SetParent(objCenter);
	backCamera->SetParent(objCenter);
	leftCamera->SetParent(objCenter);

	bossLeg1->SetParent(bossBody);
	bossLeg2->SetParent(bossBody);
	bossLeg3->SetParent(bossBody);
	bossLeg4->SetParent(bossBody);

	// FBXモデルのロード
	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// タイトル画面の初期化
	TitleInitialize();
}

void GameScreen::Update()
{
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

void GameScreen::Draw()
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

void GameScreen::TitleUpdate()
{
#pragma region 情報の取得
	SkydomRot = objSkydome->GetRotation();
	TitlePlayerPosition = objTitlePlayer->GetPosition();
	TitlePlayerRotation = objTitlePlayer->GetRotation();

	SkydomRot.y += 0.4f;

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
		if (input->TriggerKey(DIK_SPACE) || input->TriggerKey(DIK_W) || input->TriggerKey(DIK_A) || input->TriggerKey(DIK_S) || input->TriggerKey(DIK_D))
		{
			sound->PlayWav("Push.wav", Volume_Title);
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
			sound->StopWav("Title.wav");
			GameInitialize();
			scene = GAME;
		}

		break;
	}

#pragma region 情報のセット
	objSkydome->SetRotation(SkydomRot);
	objTitlePlayer->SetPosition(TitlePlayerPosition);
	objTitlePlayer->SetRotation(TitlePlayerRotation);

	LoadingBG->SetColor(loadingColor);

	TitleStartUI->SetColor(titleStartUIColor);
#pragma endregion

#pragma region 更新処理
	camera->Update();
	objSkydome->Update();
	objTitlePlayer->Update();
#pragma endregion

	// デバックテキスト
	AllDebugText();
	TitleDebugText();
}

void GameScreen::TitleDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//TitleBG->Draw();

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
	particleMan->Draw(cmdList);

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

void GameScreen::TitleInitialize()
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

	sound->PlayWav("Title.wav", Volume_Title, true);
}

void GameScreen::StageSelectUpdate()
{
	//SkydomRot = objSkydome->GetRotation();

	stage1Position = objStage1->GetPosition();
	stage2Position = objStage2->GetPosition();
	stage3Position = objStage3->GetPosition();

	loadingColor = LoadingBG->GetColor();

	if (input->TriggerKey(DIK_SPACE))
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

void GameScreen::StageSelectDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	//TitleBG->Draw();

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
	particleMan->Draw(cmdList);

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

void GameScreen::StageSelectInitialize()
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

void GameScreen::GameUpdate()
{
	// ルートを完走したら遷移
	if (startIndex >= 39)
	{
		changeColorFlag = true;
	}

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
	playerPosition = player->GetPosition();
	playerRotation = player->GetRotation();

	playerTargetPosition = SplinePosition(playerTargetCheckPoint, startIndex, timeRate);

	centerPosition = SplinePosition(playerCheckPoint, startIndex, timeRate);

	//CameraPos = camera->GetEye();

	SkydomPos = objSkydome->GetPosition();
	SkydomRot = objSkydome->GetRotation();


	if (bossBreak == false)
	{
		bossPosition = SplinePosition(bossCheckPoint, startIndex, timeRate);
	}
	else if (bossBreak == true)
	{
		bossPosition = bossBody->GetPosition();
	}

	bossRotation = bossBody->GetRotation();

	bossLeg1Position = bossLeg1->GetPosition();
	bossLeg2Position = bossLeg2->GetPosition();
	bossLeg3Position = bossLeg3->GetPosition();
	bossLeg4Position = bossLeg4->GetPosition();

	specialBulletPosition = { bossPosition.x , bossPosition.y - 5, bossPosition.z };

	loadingColor = LoadingBG->GetColor();

	playerHpGageSize = playerHpGage->GetSize();

	bossHpGageSize = bossHpGage->GetSize();
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
		loadingColor.w += 0.05f;
		changeSceneFlag = true;
	}

	if (changeSceneFlag == true)
	{
		changeSceneTimer--;
	}

	if (changeSceneTimer <= 0)
	{
		sound->StopWav("Play.wav");
		ResultInitialize();
		scene = RESULT;
	}

	// プレイヤーのHP計算
	playerHpRatio = playerHp / playerHpMax;
	playerHpGageSize.x = playerHpRatio * 320.0f;

	// ボスのHP計算
	bossHpRatio = bossHp / bossHpMax;
	bossHpGageSize.x = bossHpRatio * 530.0f;

#pragma region 弾関連
	Attack();

	if (input->TriggerKey(DIK_C))
	{
		bossHp = 0.0f;
		gameScore += 49999.0f;
	}

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

		if (bossRotation.y <= moveValue)
		{
			bossRotation.y += 1.0f;
		}

		break;

	case BOSSPATTERN::BODYLEFT:

		if (bossRotation.y >= -moveValue)
		{
			bossRotation.y -= 1.0f;
		}

		break;

	case BOSSPATTERN::FOURWAYRUSH:
		BossSpecialAttack();

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
		moveValue = 90.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 14)
	{
		moveValue = 180.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 15)
	{
		bossPattern = BODYUP;
	}
	else if (startIndex == 19)
	{
		bossPattern = BODYDOWN;
	}
	else if (startIndex == 30)
	{
		moveValue = 270.0f;
		bossPattern = BODYLEFT;
	}
	else if (startIndex == 31)
	{
		moveValue = 360.0f;
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
			if (OnCollision(bullet->GetPosition(), bossBody->GetPosition(), 0.8f, 0.8f) == true)
			{
				bossHp -= 10.0f;
				gameScore += 1000.0f;
				scoreUIMotion();
				sound->PlayWav("Hit.wav", Volume_Title);
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossPosition);
			}
		}
	}

	if (bossHp <= 0.0f)
	{
		bossBreak = true;
	}

	if (bossBreak == true)
	{
		bossPosition.y -= 0.3f;
		CreateBossParticles(bossPosition);
	}

	if (bossPosition.y <= -15.0f)
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
			if (OnCollision(bullet->GetPosition(), bossLeg1WorldPosition, 0.8f, 0.6f) == true)
			{
				sound->PlayWav("Hit.wav", Volume_Title);
				bossHp -= 5.0f;
				gameScore += 250.0f;
				scoreUIMotion();
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
	}


	if (bossLeg1Break == true)
	{
		bossLeg1Position.y -= 0.2f;
		CreateBossParticles(bossLeg1WorldPosition);
	}

	if (bossLeg1Position.y <= -20.0f)
	{
		bossLeg1Break = false;
		bossLeg1Flag = false;
	}

	// ボス部位2の当たり判定
	if (bossLeg2Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg2WorldPosition, 0.8f, 0.6f) == true)
			{
				sound->PlayWav("Hit.wav", Volume_Title);
				bossHp -= 5.0f;
				gameScore += 250.0f;
				scoreUIMotion();
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
	}


	if (bossLeg2Break == true)
	{
		bossLeg2Position.y -= 0.2f;
		CreateBossParticles(bossLeg2WorldPosition);
	}

	if (bossLeg2Position.y <= -20.0f)
	{
		bossLeg2Break = false;
		bossLeg2Flag = false;
	}

	// ボス部位3の当たり判定
	if (bossLeg3Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg3WorldPosition, 0.8f, 0.6f) == true)
			{
				sound->PlayWav("Hit.wav", Volume_Title);
				bossHp -= 5.0f;
				gameScore += 250.0f;
				scoreUIMotion();
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
	}


	if (bossLeg3Break == true)
	{
		bossLeg3Position.y -= 0.2f;
		CreateBossParticles(bossLeg3WorldPosition);
	}

	if (bossLeg3Position.y <= -20.0f)
	{
		bossLeg3Break = false;
		bossLeg3Flag = false;
	}

	// ボス部位4の当たり判定
	if (bossLeg4Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg4WorldPosition, 0.8f, 0.6f) == true)
			{
				sound->PlayWav("Hit.wav", Volume_Title);
				bossHp -= 5.0f;
				gameScore += 250.0f;
				scoreUIMotion();
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
	}

	if (bossLeg4Break == true)
	{
		bossLeg4Position.y -= 0.2f;
		CreateBossParticles(bossLeg4WorldPosition);
	}

	if (bossLeg4Position.y <= -20.0f)
	{
		bossLeg4Break = false;
		bossLeg4Flag = false;
	}

	// プレイヤーの当たり判定
	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		if (OnCollision(bullet->GetPosition(), playerWorldPosition, 0.8f, 0.6f) == true)
		{
			damageEffectAlpha = 1.0f;
			damageEffectAlphaVel = -0.06f;
			playerHp -= 10.0f;
			noDamageFlag = false;
			sound->PlayWav("Damage.wav", Volume_Title);
			bullet->deathFlag = true;
			// パーティクル生成
			CreateHitParticles(playerWorldPosition);
		}
	}

	damageEffectAlpha += damageEffectAlphaVel;

	if (damageEffectAlpha < 0.0f)
	{
		damageEffectAlphaVel = 0.0f;
		damageEffectAlpha = 0.0f;
	}

#pragma endregion

#pragma endregion

#pragma region スプライン曲線関係
	if (input->PushKey(DIK_R))
	{
		startCount = GetTickCount();
		startIndex = 1;
	}

	if (railCountFlag == true)
	{
		SplineCount();
	}

	if (startIndex == 12)
	{
		//railCountFlag = false;
	}

#pragma endregion

#pragma region 座標変換
	// XMFLOAT3 f;
	// XMVECTOR v;
	// XMMATRIX m;

	// XMFLOAT3 を XMVECTORに変換
	// v = DirectX::XMLoadFloat3(&f);
	// v.m128_f32[3] = 1.0f;

	// 座標v に行列mを掛け算
	// v = DirectX::XMVector3Transform(v, m);

	// XMVECTOR を XMFLOAT3に変換
	// DirectX::XMStoreFloat3(&f, v);

	// カメラの座標変換
	XMVECTOR cameraFrontPositionV;
	XMVECTOR cameraRightPositionV;
	XMVECTOR cameraBackPositionV;
	XMVECTOR cameraLeftPositionV;

	cameraFrontPositionV = DirectX::XMLoadFloat3(&frontCamera->GetPosition());
	cameraFrontPositionV.m128_f32[3] = 1.0f;
	cameraFrontPositionV = DirectX::XMVector3Transform(cameraFrontPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&cameraFrontPosition, cameraFrontPositionV);

	cameraRightPositionV = DirectX::XMLoadFloat3(&rightCamera->GetPosition());
	cameraRightPositionV.m128_f32[3] = 1.0f;
	cameraRightPositionV = DirectX::XMVector3Transform(cameraRightPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&cameraRightPosition, cameraRightPositionV);

	cameraBackPositionV = DirectX::XMLoadFloat3(&backCamera->GetPosition());
	cameraBackPositionV.m128_f32[3] = 1.0f;
	cameraBackPositionV = DirectX::XMVector3Transform(cameraBackPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&cameraBackPosition, cameraBackPositionV);

	cameraLeftPositionV = DirectX::XMLoadFloat3(&leftCamera->GetPosition());
	cameraLeftPositionV.m128_f32[3] = 1.0f;
	cameraLeftPositionV = DirectX::XMVector3Transform(cameraLeftPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&cameraLeftPosition, cameraLeftPositionV);

	// プレイヤーの座標変換
	XMVECTOR playerPositionV;

	playerPositionV = DirectX::XMLoadFloat3(&playerPosition);
	playerPositionV.m128_f32[3] = 1.0f;
	playerPositionV = DirectX::XMVector3Transform(playerPositionV, objCenter->GetMatWorld());
	DirectX::XMStoreFloat3(&playerWorldPosition, playerPositionV);

	// ボスの座標変換
	XMVECTOR bossLeg1PositionV;
	XMVECTOR bossLeg2PositionV;
	XMVECTOR bossLeg3PositionV;
	XMVECTOR bossLeg4PositionV;

	bossLeg1PositionV = DirectX::XMLoadFloat3(&bossLeg1Position);
	bossLeg1PositionV.m128_f32[3] = 1.0f;
	bossLeg1PositionV = DirectX::XMVector3Transform(bossLeg1PositionV, bossBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg1WorldPosition, bossLeg1PositionV);

	bossLeg2PositionV = DirectX::XMLoadFloat3(&bossLeg2Position);
	bossLeg2PositionV.m128_f32[3] = 1.0f;
	bossLeg2PositionV = DirectX::XMVector3Transform(bossLeg2PositionV, bossBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg2WorldPosition, bossLeg2PositionV);

	bossLeg3PositionV = DirectX::XMLoadFloat3(&bossLeg3Position);
	bossLeg3PositionV.m128_f32[3] = 1.0f;
	bossLeg3PositionV = DirectX::XMVector3Transform(bossLeg3PositionV, bossBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg3WorldPosition, bossLeg3PositionV);

	bossLeg4PositionV = DirectX::XMLoadFloat3(&bossLeg4Position);
	bossLeg4PositionV.m128_f32[3] = 1.0f;
	bossLeg4PositionV = DirectX::XMVector3Transform(bossLeg4PositionV, bossBody->GetMatWorld());
	DirectX::XMStoreFloat3(&bossLeg4WorldPosition, bossLeg4PositionV);


#pragma endregion

	// レールの進行方向へ自機の角度を向ける
	testX = playerTargetPosition.x - centerPosition.x;
	testZ = playerTargetPosition.z - centerPosition.z;
	testRadians = atan2(testZ, testX);
	testDegrees = XMConvertToDegrees(testRadians);

#pragma region 座標のセット
	// カメラ座標のセット
	//camera->SetEye(cameraFrontPosition);
	CameraSwitching();
	camera->SetTarget(centerPosition);

	// プレイヤー座標のセット
	player->SetPosition(playerPosition);
	// player->SetRotation({playerRotation.x, -testDegrees + 180.0f, playerRotation.z});
	player->SetRotation(playerRotation);

	// レール中心オブジェクト座標のセット
	objCenter->SetPosition(centerPosition);
	objCenter->SetRotation({ centerRotation.x, -testDegrees + 90.0f, centerRotation.z });

	// 背景天球座標のセット
	/*objSkydome->SetPosition(SkydomPos);
	objSkydome->SetRotation(SkydomRot);*/

	// ボス関連座標のセット
	bossBody->SetPosition(bossPosition);
	bossBody->SetRotation(bossRotation);

	bossLeg1->SetPosition(bossLeg1Position);
	bossLeg2->SetPosition(bossLeg2Position);
	bossLeg3->SetPosition(bossLeg3Position);
	bossLeg4->SetPosition(bossLeg4Position);

	LoadingBG->SetColor(loadingColor);

	damageEffect->SetColor({ 1.0f, 1.0f, 1.0f, damageEffectAlpha });

	playerHpGage->SetSize(playerHpGageSize);

	bossHpGage->SetSize(bossHpGageSize);
#pragma endregion

#pragma region 更新処理
	// ボス関連の更新
	bossBody->Update();
	bossLeg1->Update();
	bossLeg2->Update();
	bossLeg3->Update();
	bossLeg4->Update();

	// パーティクルの更新
	particleMan->Update();

	// 背景天球
	objSkydome->Update();

	// 地面の更新
	objGround->Update();

	// カメラの更新
	camera->Update();

	// プレイヤーの更新
	player->Update();

	frontCamera->Update();
	rightCamera->Update();
	backCamera->Update();
	leftCamera->Update();

	// レール中心オブジェクトの更新
	objCenter->Update();

	// FBXの更新
	testobject->Update();
#pragma endregion

	collisionManager->CheckAllCollisions();

	// デバックテキスト
	AllDebugText();
	GameDebugText();

	//scoreUIMotion();

	// スコアの描画
	std::ostringstream StartIndex;
	StartIndex << std::fixed << std::setprecision(0) << std::setw(7) << gameScore;
	scoreText.Print(StartIndex.str(), scoreBasePosition, { 0.760f, 0.929f, 1.0f, gameScoreAlpha }, 0.8f);

	/*std::ostringstream elapsedCounter;
	elapsedCounter << "TIME:("
		<< std::fixed << std::setprecision(2)
		<< elapsedCount << ")";
	scoreText.Print(elapsedCounter.str(), { 240, 82 }, { 1.0f, 0.882f, 0.768, gameScoreAlpha }, 0.6f);*/

	scoreUIUpdate();
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
	player->Draw();

	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Draw();
	}

	for (std::unique_ptr<BossBullet>& bullet : bossBullets)
	{
		bullet->Draw();
	}

	for (std::unique_ptr<BossTargetBullet>& bullet : bossTargetBullets)
	{
		bullet->Draw();
	}

	// objC->Draw();

	if (bossFlag == true)
	{
		bossBody->Draw();

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

	/*frontCamera->Draw();
	rightCamera->Draw();
	backCamera->Draw();
	leftCamera->Draw();*/

	// パーティクルの描画
	particleMan->Draw(cmdList);

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

	GameFG->Draw();

	//gameGTXT_1->Draw();

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

	LoadingBG->Draw();

	// デバッグテキストの描画
	// debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScreen::GameInitialize()
{
	// 座標のセット
	player->SetPosition({ 0.0f,0.0f,0.0f });
	player->SetRotation({ 0.0f, 90.0f, 0.0f });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	frontCamera->SetPosition({ 0.0f,0.0f,-20.0f });
	rightCamera->SetPosition({ -20.0f,0.0f,0.0f });
	backCamera->SetPosition({ 0.0f,0.0f,20.0f });
	leftCamera->SetPosition({ 20.0f,0.0f,0.0f });

	objCenter->SetPosition({ 0.0f,0.0f,0.0f });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	bossBody->SetPosition({ 0.0f,0.0f,20.0f });
	bossLeg1->SetPosition({ 2.0f,-2.0f,2.0f });
	bossLeg2->SetPosition({ 2.0f,-2.0f,-2.0f });
	bossLeg3->SetPosition({ -2.0f,-2.0f,2.0f });
	bossLeg4->SetPosition({ -2.0f,-2.0f,-2.0f });

	bossBody->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg1->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg2->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg3->SetRotation({ 0.0f,0.0f,0.0f });
	bossLeg4->SetRotation({ 0.0f,0.0f,0.0f });

	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetEye({ 0.0f, 0.0f, 10.0f });
	camera->SetUp({ 0.0f, 1.0f, 0.0f });

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

	scoreNull_1->SetSize({ 25.0f, 25.0f });
	scoreNull_2->SetSize({ 25.0f, 25.0f });
	scoreNull_3->SetSize({ 25.0f, 25.0f });
	scoreNull_4->SetSize({ 25.0f, 25.0f });
	scoreNull_5->SetSize({ 25.0f, 25.0f });
	scoreNull_6->SetSize({ 25.0f, 25.0f });

	scoreBasePosition = { 300.0f, 52.0f };

	// プレイヤー関連
	playerHpMax = 300.0f;
	playerHp = playerHpMax;

	// ボス関連
	bossHpMax = 500.0f;
	bossHp = bossHpMax;
	bossLeg1Hp = 10.0f;
	bossLeg2Hp = 10.0f;
	bossLeg3Hp = 10.0f;
	bossLeg4Hp = 10.0f;

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

	cameraMode = 0;

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

	gameScoreAlpha = 1.0f;

	noDamageFlag = true;
	targetScoreFlag = false;

	damageEffectAlpha = 0.0f;
	damageEffectAlphaVel = 0.0f;

	sound->PlayWav("Play.wav", Volume_Title, true);

	startCount = GetTickCount();
}

void GameScreen::ResultUpdate()
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

	if (input->TriggerKey(DIK_Q))
	{
		resultMoveVel_8 = 1.0f;
	}

	// 値の制御
	if (resultTimer > 0.0f)
	{
		// 値の設定
		resultMoveVelX_1 = 60.0f;
		resultMoveAccX_1 = 1.0f;
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
		// resultChangeAlpha_10 = 0.05f;

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
		if (input->TriggerKey(DIK_SPACE))
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
	scoreText.Print(NoDamageBonus.str(), noDamageBonusPosition, { 1.0f, 1.0f, 0.0f, noDamageBonusColor.w }, 0.5f);

	std::ostringstream TotalScone;
	TotalScone << std::fixed << std::setprecision(0) << std::setw(7) << totalScore;
	scoreText.Print(TotalScone.str(), totalScorePosition, { 0.760f, 0.929f, 1.0f, totalScoreColor.w }, 0.8f);
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

void GameScreen::ResultInitialize()
{
	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 0 });
	camera->SetUp({ 0, 1, 0 });

	// LoadingBG->SetColor({ 1, 1, 1, 0 });
	loadingColor.w = 0;

	gameScoreAlpha = 0;

	totalScore = 0;

	noDamageBonus = 20000;

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
	resultGTXT_2->SetColor({ 1.0f, 1.0f, 0.0f, 0.0f });
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

void GameScreen::GameOverUpdate()
{

	if (input->TriggerKey(DIK_SPACE))
	{
		TitleInitialize();
		scene = TITLE;
	}

	camera->Update();

	// デバックテキスト
	AllDebugText();
}

void GameScreen::GameOverDraw()
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
	//particleMan->Draw(cmdList);

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

void GameScreen::GameOverInitialize()
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

void GameScreen::CreateHitParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 0.0f;
		XMFLOAT3 pos{};
		pos.x = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.x;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = 1.0f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.0f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(20, pos, vel, acc, 3.0f, 0.0f);
	}
}

void GameScreen::CreateBossParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		// X,Y,Z全て[-20.0f,+20.0f]でランダムに分布
		const float rnd_pos = 0.0f;
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
		const float rnd_acc = 0.01f;
		acc.y = +(float)rand() / RAND_MAX * rnd_acc;

		// 追加
		particleMan->Add(30, pos, vel, acc, 2.0f, 0.0f);
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

void GameScreen::TitleDebugText()
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

void GameScreen::GameDebugText()
{
	// プレイヤーの座標を表示
	//std::ostringstream PlayerPos;
	//PlayerPos << "PlayerPos:("
	//	<< std::fixed << std::setprecision(2)
	//	<< playerPosition.x << "," // x
	//	<< playerPosition.y << "," // y
	//	<< playerPosition.z << ") Local"; // z
	//debugText.Print(PlayerPos.str(), 50, 30, 1.0f);

	//std::ostringstream bossPos;
	//bossPos << "BossPos:("
	//	<< std::fixed << std::setprecision(2)
	//	<< BossPos.x << "," // x
	//	<< BossPos.y << "," // y
	//	<< BossPos.z << ")"; // z
	//debugText.Print(bossPos.str(), 50, 50, 1.0f);

	//std::ostringstream cameraPos;
	//cameraPos << "CameraPos:("
	//	<< std::fixed << std::setprecision(2)
	//	<< CameraPos.x << "," // x
	//	<< CameraPos.y << "," // y
	//	<< CameraPos.z << ")"; // z
	//debugText.Print(cameraPos.str(), 50, 70, 1.0f);

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
		<< startIndex << ")";
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);

	// ボスのHP関連
	std::ostringstream BossHp;
	BossHp << "BossHp:("
		<< std::fixed << std::setprecision(2)
		<< bossHp << ")";
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

	// 自機操作方法
	//debugText.Print("WASD : PlayerMove", 1050, 30, 1.0f);

	//debugText.Print("SPACE : Shot", 1050, 50, 1.0f);
}

// カメラ方向の切り替え
void GameScreen::CameraSwitching()
{
	if (cameraMode == 0)
	{
		playerPosition.z = 0;

		camera->SetEye(cameraFrontPosition);
	}
	if (cameraMode == 1)
	{
		playerPosition.x = 0;

		camera->SetEye(cameraRightPosition);
	}
	if (cameraMode == 2)
	{
		playerPosition.z = 0;

		camera->SetEye(cameraBackPosition);
	}
	if (cameraMode == 3)
	{
		playerPosition.x = 0;

		camera->SetEye(cameraLeftPosition);
	}

	if (startIndex == 12)
	{
		cameraMode = 2;
	}

	else if (startIndex == 29)
	{
		cameraMode = 0;
	}
}

void GameScreen::Attack()
{
	shotRate -= 0.1f;

	XMVECTOR bulletVelocity = { 0,0,bulletSpeed };

	if (input->PushKey(DIK_SPACE) || (IsButtonPush(ButtonKind::Button_A)))
	{
		if (shotRate <= 0)
		{
			shotFlag = true;
		}

		if (shotFlag == true)
		{
			sound->PlayWav("Shot.wav", Volume_Title);

			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet = Bullet::Create(modelBullet, playerWorldPosition, bulletScale, bulletSpeed);

			bullets.push_back(std::move(newBullet));

			shotFlag = false;
			shotRate = 1.5f;
		}
	}
}

void GameScreen::BossAttack()
{
	/*std::unique_ptr<BossBullet> newBullet = std::make_unique<BossBullet>();
	newBullet = BossBullet::Create(modelBullet, bossPosition, bulletScale, bulletVelocity);

	bossBullets.push_back(std::move(newBullet));*/

	if (playerHp >= 0.0f)
	{
		sound->PlayWav("Bomb.wav", Volume_Title);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg1Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg1Flag == true))
	{
		sound->PlayWav("Bomb.wav", Volume_Title);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg1WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg2Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg2Flag == true))
	{
		sound->PlayWav("Bomb.wav", Volume_Title);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg2WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg3Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg3Flag == true))
	{
		sound->PlayWav("Bomb.wav", Volume_Title);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg3WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg4Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg4Flag == true))
	{
		sound->PlayWav("Bomb.wav", Volume_Title);
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg4WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossSpecialAttack()
{
	std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
	newBullet = BossTargetBullet::Create(modelBullet, specialBulletPosition, bulletScale, playerWorldPosition, specialBulletSpeed);

	bossTargetBullets.push_back(std::move(newBullet));
}

void GameScreen::SplineCount()
{
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
}

void GameScreen::LoadTextureFunction()
{
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/Title.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(2, L"Resources/Sprite/ResultUI/result_bg.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(3, L"Resources/Sprite/Loading.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/Sprite/StageSelect.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(5, L"Resources/Sprite/HpUI/playerHpUI.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(6, L"Resources/Sprite/HpUI/playerHpGage.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(7, L"Resources/Sprite/BossHpUI/bossHpUI.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(8, L"Resources/Sprite/BossHpUI/bossHpGage.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(9, L"Resources/Sprite/HpUI/playerHpFrame.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(11, L"Resources/Sprite/ScoreUI/scoreUI.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(12, L"Resources/Sprite/ScoreUI/scoreChar.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/Sprite/ScoreUI/scoreNull.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/Sprite/ResultUI/result_frame_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/Sprite/ResultUI/result_frame_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(16, L"Resources/Sprite/ResultUI/result_frame_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(17, L"Resources/Sprite/ResultUI/result_gtxt_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(18, L"Resources/Sprite/ResultUI/result_gtxt_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(19, L"Resources/Sprite/ResultUI/result_gtxt_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(20, L"Resources/Sprite/ResultUI/result_gtxt_4.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(21, L"Resources/Sprite/ResultUI/result_gtxt_5.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(22, L"Resources/Sprite/ResultUI/result_gtxt_6.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(23, L"Resources/Sprite/ResultUI/result_gtxt_7.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(24, L"Resources/Sprite/ResultUI/result_gtxt_8.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(25, L"Resources/Sprite/ResultUI/result_gtxt_9.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(26, L"Resources/Sprite/GameFG.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(27, L"Resources/Sprite/TitleStartUI.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(28, L"Resources/Sprite/ResultUI/result_parts_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(29, L"Resources/Sprite/ResultUI/result_parts_2.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(30, L"Resources/Sprite/ResultUI/result_parts_3.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(31, L"Resources/Sprite/ResultUI/result_parts_4.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(32, L"Resources/Sprite/ResultUI/result_parts_5.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(33, L"Resources/Sprite/ResultUI/result_gtxt_12.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(34, L"Resources/Sprite/ResultUI/result_gtxt_13.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(35, L"Resources/Sprite/ResultUI/result_gtxt_14.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(36, L"Resources/Sprite/ResultUI/result_gtxt_15.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(37, L"Resources/Sprite/ResultUI/result_parts_6.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(38, L"Resources/Sprite/BossHpUI/bossHpFrame.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(39, L"Resources/Sprite/Effect/damage_effect_1.png")) {
		assert(0);
		return;
	}
}

void GameScreen::LoadWavFunction()
{
	sound->LoadWav("Title.wav");
	sound->LoadWav("Shot.wav");
	sound->LoadWav("Bomb.wav");
	sound->LoadWav("Damage.wav");
	sound->LoadWav("Hit.wav");
	sound->LoadWav("Play.wav");
	sound->LoadWav("Push.wav");
}

void GameScreen::scoreUIMotion()
{
	scoreMoveVel = -8;
}

void GameScreen::scoreUIUpdate()
{
	scoreMoveVel += scoreMoveAcc;
	scoreBasePosition.y += scoreMoveVel;

	if (scoreBasePosition.y < 45)
	{
		scoreBasePosition.y = 45;
		scoreMoveVel = 0;
	}

	if (scoreBasePosition.y > 53)
	{
		scoreBasePosition.y = 53;
		scoreMoveVel = 0;
	}
}

bool GameScreen::OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB)
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

XMFLOAT3 GameScreen::lerp(const XMFLOAT3& start, const XMFLOAT3& end, const float t)
{
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 C;
	C = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return C;
}
