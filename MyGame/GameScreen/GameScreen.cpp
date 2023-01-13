﻿#include "GameScreen.h"

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

	//音声の読み込み
	LoadWavFunction();

	// テクスチャ読み込み
	LoadTextureFunction();

	// スプライト生成
	TitleBG = Sprite::Create(1, { 0.0f,0.0f });
	ResultBG = Sprite::Create(2, { 0.0f,0.0f });
	TitleLogo = Sprite::Create(1, { 100.0f,100.0f }, { 1, 1, 1, 1 });
	LoadingBG = Sprite::Create(3, { 0.0f,0.0f }, { 1,1,1,0 });
	StageSelectBG = Sprite::Create(4, { 0.0f,0.0f });


	GameFG = Sprite::Create(26, { 0.0f,0.0f });
	TitleStartUI = Sprite::Create(27, { 310.0f,630.0f });

	playerHpUI = Sprite::Create(5, { 810.0f, 540.0f });
	playerHpGage = Sprite::Create(6, { 833.0f, 680.0f });


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

	case SCENE::STAGESELECT:
		StageSelectDraw();
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
		moveY -= 0.001;

		if (moveY <= -0.01)
		{
			subSpeedY = false;
		}

	}
	if (addSpeedY == true)
	{
		moveY += 0.001;

		if (moveY >= 0.01)
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
			moveX -= 0.001;

			if (moveX <= -0.05)
			{
				subSpeedX = false;
			}

		}
		if (addSpeedX == true)
		{
			moveX += 0.001;

			if (moveX >= 0.05)
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

		if (moveX >= 0)
		{
			moveX -= 0.001;
		}
		if (moveX <= 0)
		{
			moveX += 0.001;
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
			moveX -= 0.005;
			if (backTimer <= 0.0f)
			{
				stagingFlag = false;
			}
		}

		if (stagingFlag == false)
		{
			if (TitlePlayerPosition.x <= 50)
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

	LoadingBG->SetColor({ 1, 1, 1, 0 });
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
	if (playerHp <= 0)
	{
		changeColorFlag = true;
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

#pragma region 弾関連
	Attack();

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
	if (startIndex == 4)
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
				bossHp -= 1;
				sound->PlayWav("Hit.wav", Volume_Title);
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossPosition);
			}
		}
	}

	if (bossHp <= 0)
	{
		bossBreak = true;
	}

	if (bossBreak == true)
	{
		bossPosition.y -= 0.2f;
		CreateBossParticles(bossPosition);
	}

	if (bossPosition.y <= -20.0f)
	{
		bossBreak == false;

		bossFlag = false;
		bossLeg1Flag == false;
		bossLeg2Flag == false;
		bossLeg3Flag == false;
		bossLeg4Flag == false;
	}

	// ボス部位1の当たり判定
	if (bossLeg1Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg1WorldPosition, 0.8f, 0.6f) == true)
			{
				sound->PlayWav("Hit.wav", Volume_Title);
				bossHp -= 1;
				if (rushFlag == false)
				{
					bossLeg1Hp -= 1;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg1WorldPosition);
			}
		}
	}

	if (bossLeg1Hp <= 0)
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
				bossHp -= 1;
				if (rushFlag == false)
				{
					bossLeg2Hp -= 1;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg2WorldPosition);
			}
		}
	}

	if (bossLeg2Hp <= 0)
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
				bossHp -= 1;
				if (rushFlag == false)
				{
					bossLeg3Hp -= 1;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg3WorldPosition);
			}
		}
	}

	if (bossLeg3Hp <= 0)
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
				bossHp -= 1;
				if (rushFlag == false)
				{
					bossLeg4Hp -= 1;
				}
				bullet->deathFlag = true;
				// パーティクル生成
				CreateHitParticles(bossLeg4WorldPosition);
			}
		}
	}

	if (bossLeg4Hp <= 0)
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
			playerHp -= 10;
			playerHpGageSize.x -= 10;
			sound->PlayWav("Damage.wav", Volume_Title);
			bullet->deathFlag = true;
			// パーティクル生成
			CreateHitParticles(playerWorldPosition);
		}
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

	playerHpGage->SetSize(playerHpGageSize);
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

	// 描画
	playerHpUI->Draw();
	playerHpGage->Draw();

	GameFG->Draw();

	LoadingBG->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GameScreen::GameInitialize()
{
	// 座標のセット
	player->SetPosition({ 0,0,0 });
	player->SetRotation({ 0, 90, 0 });
	player->SetScale({ 1.0f, 1.0f, 1.0f });

	objSkydome->SetPosition({ 0.0f, 0.0f, 0.0f });
	objSkydome->SetRotation({ 0.0f,0.0f,0.0f, });
	objSkydome->SetScale({ 5.0f, 5.0f, 5.0f });

	frontCamera->SetPosition({ 0,0,-20.0f });
	rightCamera->SetPosition({ -20,0,0.0f });
	backCamera->SetPosition({ 0,0,20.0f });
	leftCamera->SetPosition({ 20,0,0.0f });

	objCenter->SetPosition({ 0,0,0 });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	bossBody->SetPosition({ 0,0,20 });
	bossLeg1->SetPosition({ 2,-2,2 });
	bossLeg2->SetPosition({ 2,-2,-2 });
	bossLeg3->SetPosition({ -2,-2,2 });
	bossLeg4->SetPosition({ -2,-2,-2 });

	bossBody->SetRotation({ 0,0,0 });
	bossLeg1->SetRotation({ 0,0,0 });
	bossLeg2->SetRotation({ 0,0,0 });
	bossLeg3->SetRotation({ 0,0,0 });
	bossLeg4->SetRotation({ 0,0,0 });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	LoadingBG->SetColor({ 1, 1, 1, 1.0f });
	//loadingColor.w = 1.0f;

	playerHpGage->SetSize({ 320.0f, 30 });

	// プレイヤー関連
	playerHp = 320;

	// ボス関連
	bossHp = 60;
	bossLeg1Hp = 40;
	bossLeg2Hp = 40;
	bossLeg3Hp = 40;
	bossLeg4Hp = 40;

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

	sound->PlayWav("Play.wav", Volume_Title, true);

	startCount = GetTickCount();
}

void GameScreen::ResultUpdata()
{
	loadingColor = LoadingBG->GetColor();

	if (changeColorFlag == false)
	{
		if (loadingColor.w > -0.1)
		{
			loadingColor.w -= 0.05f;
		}
	}

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
		loadingColor.w = 0.0f;
		TitleInitialize();
		scene = TITLE;
	}

	LoadingBG->SetColor(loadingColor);

	camera->Update();

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

	LoadingBG->SetColor({ 1, 1, 1, 1 });

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
	debugText.Print(BossHp.str(), 50, 30, 1.0f);

	std::ostringstream BossLeg1Hp;
	BossLeg1Hp << "BossLeg1Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg1Hp << ")";
	debugText.Print(BossLeg1Hp.str(), 50, 50, 1.0f);

	std::ostringstream BossLeg2Hp;
	BossLeg2Hp << "BossLeg2Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg2Hp << ")";
	debugText.Print(BossLeg2Hp.str(), 50, 70, 1.0f);

	std::ostringstream BossLeg3Hp;
	BossLeg3Hp << "BossLeg3Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg3Hp << ")";
	debugText.Print(BossLeg3Hp.str(), 50, 90, 1.0f);

	std::ostringstream BossLeg4Hp;
	BossLeg4Hp << "BossLeg4Hp:("
		<< std::fixed << std::setprecision(2)
		<< bossLeg4Hp << ")";
	debugText.Print(BossLeg4Hp.str(), 50, 110, 1.0f);

	std::ostringstream PlayerHp;
	PlayerHp << "PlayerHp:("
		<< std::fixed << std::setprecision(2)
		<< playerHp << ")";
	debugText.Print(PlayerHp.str(), 50, 130, 1.0f);

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
	debugText.Print("WASD : PlayerMove", 1050, 30, 1.0f);

	debugText.Print("SPACE : Shot", 1050, 50, 1.0f);
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
			sound->PlayWav("Shot.wav", Volume_Title);
		}

		if (shotFlag == true)
		{
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

	sound->PlayWav("Bomb.wav", Volume_Title);

	if (playerHp >= 0)
	{
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg1Attack()
{
	sound->PlayWav("Bomb.wav", Volume_Title);
	if (playerHp >= 0)
	{
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg1WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg2Attack()
{
	sound->PlayWav("Bomb.wav", Volume_Title);
	if (playerHp >= 0)
	{
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg2WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg3Attack()
{
	sound->PlayWav("Bomb.wav", Volume_Title);
	if (playerHp >= 0)
	{
		std::unique_ptr<BossTargetBullet> newBullet = std::make_unique<BossTargetBullet>();
		newBullet = BossTargetBullet::Create(modelBullet, bossLeg3WorldPosition, bulletScale, playerWorldPosition, bulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GameScreen::BossLeg4Attack()
{
	sound->PlayWav("Bomb.wav", Volume_Title);
	if (playerHp >= 0)
	{
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

	/*if (timeRate >= 1.0f)
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
	}*/
}

void GameScreen::LoadTextureFunction()
{
	if (!Sprite::LoadTexture(1, L"Resources/Sprite/Title.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(2, L"Resources/Sprite/ResultBG.png")) {
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

	if (!Sprite::LoadTexture(26, L"Resources/Sprite/GameFG.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(27, L"Resources/Sprite/TitleStartUI.png")) {
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
