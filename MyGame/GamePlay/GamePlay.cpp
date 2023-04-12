#include "GamePlay.h"

extern int cameraMode = 0;
extern bool playerUpdateFlag = false;

extern float gameScore = 0.0f;
extern float gameScoreMax = 9999999.0f;

// ノーダメージフラグ
extern bool noDamageFlag = true;
// 目標スコアに達しているかフラグ
extern bool targetScoreFlag = false;
// 全ての部位を破壊したか
extern bool allLegBreakFlag = false;

//オーディオのボリューム
extern float masterVolumeNow = 7.0f;
extern float masterVolume = 0.0f;

extern float bgmVolumeMax = 10.0f;
extern float bgmVolumeNow = 10.0f;
extern float bgmVolumeRatio = 0.0f;
extern float bgmVolume = 0.0f;

extern float seVolumeMax = 10.0f;
extern float seVolumeNow = 10.0f;
extern float seVolumeRatio = 0.0f;
extern float seVolume = 0.0f;

using namespace DirectX;

GamePlay::GamePlay()
{
}

GamePlay::~GamePlay()
{
}

void GamePlay::Initialize()
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

	// パーティクルマネージャー
	bossHitParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect6.png");
	bossBreakParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	playerJetParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	playerContrailParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	bulletParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");
	stageBoxParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect2.png");

	// 3Dオブジェクト生成
	objSkydome = ObjObject::Create();
	objGround = ObjObject::Create();

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

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");
	modelBossLeg = ObjModel::CreateFromOBJ("bossLeg");
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

	bossBody->SetModel(modelBossCore);
	bossUpperBody->SetModel(modelBossUpperBody);
	bossLowerBody->SetModel(modelBossLowerBody);
	bossLeg1->SetModel(modelBossLeg);
	bossLeg2->SetModel(modelBossLeg);
	bossLeg3->SetModel(modelBossLeg);
	bossLeg4->SetModel(modelBossLeg);

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

	// FBXモデルのロード
	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// 各音量初期化
	VolumeCalc();

	GameInitialize();
}

void GamePlay::Finalize()
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
}

void GamePlay::Update()
{
	// 音量の計算
	VolumeCalc();

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

	moveCameraPosition_1 = Easing::OutQuadFloat3({ 5.0f, 0.0f,-1.0f }, { 4.0f, 3.0f,2.0f }, L5nowCount);
	moveCameraPosition_2 = Easing::OutQuadFloat3({ 2.0f, -1.0f,-5.0f }, { -4.0f, 3.0f,-2.0f }, L5nowCount);
	moveCameraPosition_3 = Easing::OutQuadFloat3({ 0.0f, 2.0f,-10.0f }, { 0.0f,0.0f,-20.0f }, L5nowCount);

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
		//ResultInitialize();
		SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	if (Input::GetInstance()->TriggerKey(DIK_C))
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
	}

	playerHpCalc();

	bossHpCalc();

	alertUIUpdate();

	CreateBoxParticle();

	// 雲を更新
	for (std::unique_ptr<StageObject>& box : stageObjects)
	{
		box->Update();
	}

	// 雲を消去
	stageObjects.remove_if([](std::unique_ptr<StageObject>& cloud)
		{
			return cloud->GetDeathFlag();
		}
	);

#pragma region 弾関連
	Attack();

	chargeAttack();

	homingAttack();

	// 弾を更新
	for (std::unique_ptr<Bullet>& bullet : playerBullets)
	{
		bullet->Update();
	}

	// 弾を消去
	playerBullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// プレイヤーの狙い弾を更新
	for (std::unique_ptr<TargetBullet>& bullet : playerTargetBullets)
	{
		bullet->Update();
	}

	// プレイヤーの狙い弾を消去
	playerTargetBullets.remove_if([](std::unique_ptr<TargetBullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// 弾を更新
	for (std::unique_ptr<Bullet>& bullet : playerChargeBullets)
	{
		bullet->Update();
	}

	// 弾を消去
	playerChargeBullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);

	// ボスの狙い弾を更新
	for (std::unique_ptr<TargetBullet>& bullet : bossTargetBullets)
	{
		bullet->Update();
	}

	// ボスの狙い弾を消去
	bossTargetBullets.remove_if([](std::unique_ptr<TargetBullet>& bullet)
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			if (OnCollision(bullet->GetPosition(), bossWorldPosition, 0.6f, 1.5f) == true)
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg1WorldPosition, 0.6f, 0.8f) == true)
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg2WorldPosition, 0.6f, 0.8f) == true)
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg3WorldPosition, 0.6f, 0.8f) == true)
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg4WorldPosition, 0.6f, 0.8f) == true)
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
	for (std::unique_ptr<TargetBullet>& bullet : bossTargetBullets)
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

	for (std::unique_ptr<Bullet>& bullet : playerBullets)
	{
		CreateBulletParticles(bullet->GetPosition(), { 0.1f,1.0f, 0.1f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 0.7f);
	}

	for (std::unique_ptr<TargetBullet>& bullet : playerTargetBullets)
	{
		CreateBulletParticles(bullet->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
	}

	for (std::unique_ptr<Bullet>& bullet : playerChargeBullets)
	{
		CreateChargeBulletParticles(bullet->GetPosition(), { 1.0f,1.0f, 0.1f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, 2.0f);
	}

	for (std::unique_ptr<TargetBullet>& bullet : bossTargetBullets)
	{
		CreateBulletParticles(bullet->GetPosition(), { 1.0f,0.1f, 0.1f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } , 0.7f);
	}

	for (std::unique_ptr<StageObject>& box : stageObjects)
	{
		CreateStageBoxParticles (box->GetPosition(), { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 10.0f);
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
	bulletParticle->Update();
	stageBoxParticle->Update();
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

void GamePlay::Draw()
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
		for (std::unique_ptr<Bullet>& bullet : playerBullets)
		{
			//bullet->Draw();
		}

		for (std::unique_ptr<Bullet>& bullet : playerChargeBullets)
		{
			//bullet->Draw();
		}
	}

	for (std::unique_ptr<StageObject>& box : stageObjects)
	{
		//box->Draw();
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

	//objCamera->Draw();

	// パーティクルの描画
	bossHitParticle->Draw(cmdList);
	bossBreakParticle->Draw(cmdList);
	playerJetParticle->Draw(cmdList);
	playerContrailParticle->Draw(cmdList);
	stageBoxParticle->Draw(cmdList);
	if (playerUpdateFlag == true)
	{
		bulletParticle->Draw(cmdList);
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

	//player->DebugTextDraw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}

void GamePlay::GameInitialize()
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

	bossBody->SetScale({ 3.5f, 3.5f, 3.5f });
	bossLeg1->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg2->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg3->SetScale({ 0.8f, 0.8f, 0.8f });
	bossLeg4->SetScale({ 0.8f, 0.8f, 0.8f });

	camera->SetTarget({ 0.0f, 0.0f, 0.0f });
	camera->SetEye({ 0.0f, 0.0f, 10.0f });
	camera->SetUp({ 0.0f, 1.0f, 0.0f });

	// シーン遷移時の画面暗転
	LoadingBG->SetColor({ 1, 1, 1, 1.0f });
	//loadingColor.w = 1.0f;

	// プレイヤーのHPゲージ
	playerHpGage->SetColor({ 0.1f, 0.6f, 0.1f, 1.0f });
	playerHpGage->SetSize({ 320.0f, 30.0f });
	playerHpGage->SetAnchorPoint({ 1.0f, 0.5f });

	playerDamageGage->SetColor({ 1.0f, 0, 0.2f, 1.0f });
	playerDamageGage->SetSize({ 320.0f, 30.0f });
	playerDamageGage->SetAnchorPoint({ 1.0f, 0.5f });

	playerHpUI->SetAnchorPoint({ 1.0f,0.5f });

	playerHpUICover->SetAnchorPoint({ 1.0f, 0.5f });

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

void GamePlay::CreateHitParticles(XMFLOAT3 position)
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

void GamePlay::CreateBossParticles(XMFLOAT3 position)
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

void GamePlay::CreatePlayerJetParticles(XMFLOAT3 position)
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

void GamePlay::CreatePlayerContrailParticles(XMFLOAT3 position)
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

void GamePlay::CreateBulletParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale)
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		bulletParticle->Add(7, pos, vel, acc, start_color, end_color, start_scale, 0.0f);
	}
}

void GamePlay::CreateChargeBulletParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale)
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		bulletParticle->Add(3, pos, vel, acc, start_color, end_color, start_scale, 0.0f);
	}
}

void GamePlay::CreateStageBoxParticles(XMFLOAT3 position, XMFLOAT4 start_color, XMFLOAT4 end_color, float start_scale)
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;

		XMFLOAT3 vel{};

		XMFLOAT3 acc{};

		// 追加
		stageBoxParticle->Add(7, pos, vel, acc, start_color, end_color, start_scale, 0.0f);
	}
}

void GamePlay::GameDebugText()
{
	// プレイヤーの座標を表示
	std::ostringstream PlayerPos;
	PlayerPos << "PlayerPos:("
		<< std::fixed << std::setprecision(2)
		<< playerRotation.x << "," // x
		<< playerRotation.y << "," // y
		<< playerRotation.z << ") Local"; // z
	debugText.Print(PlayerPos.str(), 50, 70, 1.0f);

	std::ostringstream bossPos;
	bossPos << "BossPos:("
		<< std::fixed << std::setprecision(2)
		<< bossWorldPosition.x << "," // x
		<< bossRotation.y << "," // y
		<< bossWorldPosition.z << ")"; // z
	debugText.Print(bossPos.str(), 50, 350, 1.0f);

	std::ostringstream StartIndex;
	StartIndex << "StartIndex:("
		<< std::fixed << std::setprecision(2)
		<< playerChargeFlag << ")";
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);

	// ボスのHP関連
	std::ostringstream BossHp;
	BossHp << "playerChargeNow:("
		<< std::fixed << std::setprecision(2)
		<< playerHomingNow << ")";
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
}

// カメラ方向の切り替え
void GamePlay::CameraSwitching()
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

void GamePlay::Attack()
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

				playerBullets.push_back(std::move(newBullet));

				shotFlag = false;
				shotRate = 1.5f;
			}
		}
	}
}

void GamePlay::chargeAttack()
{
	if (playerUpdateFlag == true)
	{
		playerChargeRatio = playerChargeNow / playerChargeMax;
		chargeBulletSize = Easing::InOutQuadFloat(0.0, 2.0, playerChargeRatio);

		CreateChargeBulletParticles(playerWorldPosition, { 1.0f,1.0f, 0.1f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, chargeBulletSize);

		if (playerChargeFlag == false)
		{
			if (playerChargeNow < playerChargeMax)
			{
				if (Input::GetInstance()->PushKey(DIK_M))
				{
					playerChargeNow++;
				}
				else if (!Input::GetInstance()->PushKey(DIK_M))
				{
					playerChargeNow--;
				}
			}
			else
			{
				if (!Input::GetInstance()->PushKey(DIK_M))
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

			playerChargeBullets.push_back(std::move(newBullet));

			playerChargeNow = 0.0f;
			playerChargeFlag = false;
		}
		playerChargeNow = max(playerChargeNow, 0.0);
		playerChargeNow = min(playerChargeNow, playerChargeMax);
	}
}

void GamePlay::homingAttack()
{
	if (playerUpdateFlag == true)
	{
		if (playerHomingNow >= 30.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x, playerWorldPosition.y + 1.0f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}
		if (playerHomingNow >= 60.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x + 0.8f, playerWorldPosition.y + 0.5f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}
		if (playerHomingNow >= 90.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x + 0.8f, playerWorldPosition.y - 0.5f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}
		if (playerHomingNow >= 120.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x, playerWorldPosition.y - 1.0f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}
		if (playerHomingNow >= 150.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x - 0.8f, playerWorldPosition.y - 0.5f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}
		if (playerHomingNow >= 180.0f)
		{
			CreateChargeBulletParticles({ playerWorldPosition.x - 0.8f, playerWorldPosition.y + 0.5f, playerWorldPosition.z }, { 0.1f,1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, 0.7f);
		}


		if (playerHomingFlag == false)
		{
			if (playerHomingNow < playerHomingMax)
			{
				if (Input::GetInstance()->PushKey(DIK_N))
				{
					playerHomingNow++;
				}
				else if (!Input::GetInstance()->PushKey(DIK_N))
				{
					playerHomingNow = 0.0f;
				}
			}
			else
			{
				if (!Input::GetInstance()->PushKey(DIK_N))
				{
					playerHomingFlag = true;
				}
			}
		}
		else
		{


			if (playerHomingNow == 180.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x - 0.8f, playerWorldPosition.y + 0.5f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}
			if (playerHomingNow == 150.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x - 0.8f, playerWorldPosition.y - 0.5f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}
			if (playerHomingNow == 120.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x, playerWorldPosition.y - 1.0f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}
			if (playerHomingNow == 90.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x + 0.8f, playerWorldPosition.y - 0.5f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}
			if (playerHomingNow == 60.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x + 0.8f, playerWorldPosition.y + 0.5f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}
			if (playerHomingNow == 30.0f)
			{
				Sound::GetInstance()->PlayWav("SE/Game/game_player_shot.wav", seVolume);

				std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
				newBullet = TargetBullet::Create(modelBullet, { playerWorldPosition.x, playerWorldPosition.y + 1.0f, playerWorldPosition.z }, bulletScale, bossWorldPosition, playerBulletSpeed);

				playerTargetBullets.push_back(std::move(newBullet));
			}

			playerHomingNow -= 2.0f;

			if (playerHomingNow <= 0.0f)
			{
				playerHomingNow = 0.0f;
				playerHomingFlag = false;
			}
		}
		playerHomingNow = max(playerHomingNow, 0.0);
		playerHomingNow = min(playerHomingNow, playerHomingMax);
	}
}

void GamePlay::BossAttack()
{
	if (playerHp >= 0.0f)
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, bossWorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GamePlay::BossLeg1Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg1Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, bossLeg1WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GamePlay::BossLeg2Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg2Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, bossLeg2WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GamePlay::BossLeg3Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg3Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, bossLeg3WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GamePlay::BossLeg4Attack()
{
	if ((playerHp >= 0.0f) && (bossLeg4Flag == true))
	{
		Sound::GetInstance()->PlayWav("SE/Game/game_boss_shot.wav", seVolume);
		std::unique_ptr<TargetBullet> newBullet = std::make_unique<TargetBullet>();
		newBullet = TargetBullet::Create(modelBullet, bossLeg4WorldPosition, bulletScale, playerWorldPosition, bossBulletSpeed);

		bossTargetBullets.push_back(std::move(newBullet));
	}
}

void GamePlay::SplineCount()
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

void GamePlay::Lerp1Count()
{
	L1nowCount = GetTickCount();

	L1elapsedCount = L1nowCount - L1startCount;
	L1elapsedTime = static_cast<float> (L1elapsedCount) / 1000.0f;

	L1timeRate = min(L1elapsedCount / L1maxTime, 1.0f);
}

void GamePlay::Lerp2Count()
{
	L2nowCount = GetTickCount();

	L2elapsedCount = L2nowCount - L2startCount;
	L2elapsedTime = static_cast<float> (L2elapsedCount) / 1000.0f;

	L2timeRate = min(L2elapsedCount / L2maxTime, 1.0f);
}

void GamePlay::Lerp3Count()
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

void GamePlay::Lerp4Count()
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

void GamePlay::Lerp5Count()
{
	L5nowCount += L5addCount;

	if (L5nowCount > 1.0f)
	{
		L5nowCount = 0.0f;
		moveCameraNumber++;
	}

	if (moveCameraNumber > 4)
	{
		L5nowCount = 1.0f;
		L5addCount = 0.0f;
	}
	else
	{
		L5addCount = 0.016f;
	}
}

void GamePlay::RankTimer()
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

void GamePlay::ScoreRateTimer()
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

void GamePlay::LoadTextureFunction()
{
	// 共通

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
}

void GamePlay::LoadWavFunction()
{
	Sound::GetInstance()->LoadWav("SE/Game/game_player_shot.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_boss_shot.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_player_damage.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_boss_damage.wav");
	Sound::GetInstance()->LoadWav("BGM/Game/game_bgm.wav");
	Sound::GetInstance()->LoadWav("SE/Game/game_alert.wav");
}

void GamePlay::scoreUIMotion()
{
	scoreMoveVel = -8;
}

void GamePlay::scoreUIUpdate()
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
void GamePlay::changeGameUIAlpha()
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
void GamePlay::damageEffectUpdate()
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
void GamePlay::alertUIUpdate()
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
void GamePlay::railTargetCalc()
{
	// レールの進行方向へ自機の角度を向ける
	testX = playerTargetPosition.x - centerPosition.x;
	testZ = playerTargetPosition.z - centerPosition.z;
	testRadians = atan2(testZ, testX);
	testDegrees = XMConvertToDegrees(testRadians);
}

// プレイヤーHPの計算
void GamePlay::playerHpCalc()
{
	// プレイヤーのHP計算
	playerHpRatio = playerHp / playerHpMax;
	playerHpGageSize.x = playerHpRatio * 320.0f;
}

// ボスHPの計算
void GamePlay::bossHpCalc()
{
	// ボスのHP計算
	bossHpRatio = bossHp / bossHpMax;
	bossHpGageSize.x = bossHpRatio * 530.0f;
}

// 各音量の計算
void GamePlay::VolumeCalc()
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

bool GamePlay::OnCollision(XMFLOAT3 sphereA, XMFLOAT3 sphereB, float radiusA, float radiusB)
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

void GamePlay::CreateBoxParticle()
{
	XMFLOAT3 randPos;

	randPos.x = ((float)rand() / RAND_MAX * 128.0f - 128.0f / 2.0f);
	randPos.y = ((float)rand() / RAND_MAX * 72.0f - 72.0f / 2.0f);
	randPos.z = centerPosition.z + 300.0f;

	//タイマーを更新
	const float stageBoxTime = 30;
	stageBoxTimer++;
	const float stageBoxTimeRate = stageBoxTimer / stageBoxTime;

	if (stageBoxTimer >= stageBoxTime)
	{
		std::unique_ptr<StageObject> newBox = std::make_unique<StageObject>();
		newBox = StageObject::Create(modelBullet, randPos, { 1.0f,1.0f,1.0f }, 1.5f);

		stageObjects.push_back(std::move(newBox));

		stageBoxTimer = 0;
	}
}