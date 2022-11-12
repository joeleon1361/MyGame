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
	player = Player::Create();

	objCenter = ObjObject::Create();

	bossBody = Boss::Create();
	bossLeg1 = Boss::Create();
	bossLeg2 = Boss::Create();
	bossLeg3 = Boss::Create();
	bossLeg4 = Boss::Create();
	objC = ObjObject::Create();

	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	modelGround = ObjModel::CreateFromOBJ("ground");
	modelPlayer = ObjModel::CreateFromOBJ("player2");
	modelBullet = ObjModel::CreateFromOBJ("bullet2");

	objSkydome->SetModel(modelSkydome);
	objGround->SetModel(modelGround);
	player->SetModel(modelPlayer);
	objCenter->SetModel(modelBullet);

	bossBody->SetModel(modelBullet);
	bossLeg1->SetModel(modelBullet);
	bossLeg2->SetModel(modelBullet);
	bossLeg3->SetModel(modelBullet);
	bossLeg4->SetModel(modelBullet);

	bossBody->SetCollider(new SphereCollider);
	bossLeg1->SetCollider(new SphereCollider);
	bossLeg2->SetCollider(new SphereCollider);
	bossLeg3->SetCollider(new SphereCollider);
	bossLeg4->SetCollider(new SphereCollider);

	objC->SetModel(modelPlayer);

	player->SetParent(objCenter);

	bossLeg1->SetParent(bossBody);
	bossLeg2->SetParent(bossBody);
	bossLeg3->SetParent(bossBody);
	bossLeg4->SetParent(bossBody);

	objC->SetParent(player);

	testmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	// FBX3Dオブジェクト生成とモデルとセット
	testobject = new FbxObject3d;
	testobject->Initialize();
	testobject->SetModel(testmodel);

	// 座標のセット
	player->SetPosition({ 0,0,0 });
	player->SetRotation({ 0, 90, 0 });
	player->SetScale({ 0.5f, 0.5f, 0.5f });

	objCenter->SetPosition({ 10,10,10 });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	objSkydome->SetPosition({ -70,0,0 });

	bossBody->SetPosition({ 0,0,0 });
	bossLeg1->SetPosition({ 2,-2,2 });
	bossLeg2->SetPosition({ 2,-2,-2 });
	bossLeg3->SetPosition({ -2,-2,2 });
	bossLeg4->SetPosition({ -2,-2,-2 });

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

	playerPosition = player->GetPosition();
	playerRotation = player->GetRotation();

	/*centerPosition = SplinePosition(playerCheckPoint, startIndex, timeRate);

	CameraPos = { centerPosition.x, centerPosition.y,centerPosition.z - 10 };

	SkydomPos = objSkydome->GetPosition();
	SkydomRot = objSkydome->GetRotation();

	bossPosition = SplinePosition(bossCheckPoint, startIndex, timeRate);
	bossRotation = bossBody->GetRotation();*/

	centerPosition = objCenter->GetPosition();

	bossPosition = bossBody->GetPosition();
	bossRotation = bossBody->GetRotation();

	bossLeg1Position = bossLeg1->GetPosition();
	bossLeg2Position = bossLeg2->GetPosition();
	bossLeg3Position = bossLeg3->GetPosition();
	bossLeg4Position = bossLeg4->GetPosition();

	XMFLOAT3 CRot = objC->GetRotation();

	// パーティクル生成
	CreateParticles();

	//CRot.x += 2;
	//bossRotation.y += 2;

	CameraSwitching();

#pragma region 弾関連
	Attack();

	// 弾を更新
	for (std::unique_ptr<Bullet>& bullet : bullets)
	{
		bullet->Update();
	}

	// アップデート
		// 弾を消去
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet)
		{
			return bullet->GetDeathFlag();
		}
	);
#pragma endregion

#pragma region ボス関連
	// 当たり判定
	if (bossFlag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossBody->GetPosition(), 0.8f, 0.8f) == true)
			{
				bossHp -= 1;
				bullet->deathFlag = true;
			}
		}
	}

	if (bossHp <= 0)
	{
		bossFlag = false;
	}

	// 当たり判定
	if (bossLeg1Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg1WorldPosition, 0.8f, 0.6f) == true)
			{
				bossLeg1Hp -= 1;
				bullet->deathFlag = true;
			}
		}
	}

	if (bossLeg1Hp <= 0)
	{
		bossLeg1Break = true;
	}


	if (bossLeg1Break == true)
	{
		bossLeg1Position.y -= 0.3f;
	}

	if (bossLeg1Position.y <= -20.0f)
	{
		bossLeg1Break = false;
		bossLeg1Flag = false;
	}

	// 当たり判定
	if (bossLeg2Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg2WorldPosition, 0.8f, 0.6f) == true)
			{
				bossLeg2Hp -= 1;
				bullet->deathFlag = true;
			}
		}
	}

	if (bossLeg2Hp <= 0)
	{
		bossLeg2Break = true;
	}


	if (bossLeg2Break == true)
	{
		bossLeg2Position.y -= 0.3f;
	}

	if (bossLeg2Position.y <= -20.0f)
	{
		bossLeg2Break = false;
		bossLeg2Flag = false;
	}

	// 当たり判定
	if (bossLeg3Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg3WorldPosition, 0.8f, 0.6f) == true)
			{
				bossLeg3Hp -= 1;
				bullet->deathFlag = true;
			}
		}
	}

	if (bossLeg3Hp <= 0)
	{
		bossLeg3Break = true;
	}


	if (bossLeg3Break == true)
	{
		bossLeg3Position.y -= 0.3f;
	}

	if (bossLeg3Position.y <= -20.0f)
	{
		bossLeg3Break = false;
		bossLeg3Flag = false;
	}

	// 当たり判定
	if (bossLeg4Flag == true)
	{
		for (std::unique_ptr<Bullet>& bullet : bullets)
		{
			if (OnCollision(bullet->GetPosition(), bossLeg4WorldPosition, 0.8f, 0.6f) == true)
			{
				bossLeg4Hp -= 1;
				bullet->deathFlag = true;
			}
		}
	}

	if (bossLeg4Hp <= 0)
	{
		bossLeg4Break = true;
	}

	if (bossLeg4Break == true)
	{
		bossLeg4Position.y -= 0.3f;
	}

	if (bossLeg4Position.y <= -20.0f)
	{
		bossLeg4Break = false;
		bossLeg4Flag = false;
	}
#pragma endregion

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

	// プレイヤーの座標変換
	XMVECTOR playerPositionV;

	playerPositionV = DirectX::XMLoadFloat3(&playerPosition);
	playerPositionV.m128_f32[3] = 1.0f;

	playerPositionV = DirectX::XMVector3Transform(playerPositionV, objCenter->GetMatWorld());

	DirectX::XMStoreFloat3(&playerBulletPosition, playerPositionV);

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

	player->SetPosition(playerPosition);
	player->SetRotation(playerRotation);

	camera->SetEye(CameraPos);
	camera->SetTarget(centerPosition);

	objCenter->SetPosition(centerPosition);

	objSkydome->SetPosition(SkydomPos);
	objSkydome->SetRotation(SkydomRot);

	bossBody->SetPosition(bossPosition);
	bossBody->SetRotation(bossRotation);

	bossLeg1->SetPosition(bossLeg1Position);
	bossLeg2->SetPosition(bossLeg2Position);
	bossLeg3->SetPosition(bossLeg3Position);
	bossLeg4->SetPosition(bossLeg4Position);

	objC->SetRotation(CRot);

	camera->Update();
	particleMan->Update();

	objSkydome->Update();
	objGround->Update();

	testobject->Update();

	objCenter->Update();
	objC->Update();
	player->Update();


	bossBody->Update();
	bossLeg1->Update();
	bossLeg2->Update();
	bossLeg3->Update();
	bossLeg4->Update();

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

	// objC->Draw();

	if (bossFlag == true)
	{
		bossBody->Draw();

		bossLeg1->Draw();

		if (bossLeg2Flag == true)
		{
			bossLeg2->Draw();
		}

		bossLeg3->Draw();

		if (bossLeg4Flag == true)
		{
			bossLeg4->Draw();
		}
	}
	
	// testobject->Draw(cmdList);

	// objCenter->Draw();

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
	player->SetPosition({ 0,0,0 });
	player->SetRotation({ 0, 90, 0 });
	player->SetScale({ 0.5f, 0.5f, 0.5f });

	objCenter->SetPosition({ 0,0,0 });
	objCenter->SetScale({ 0.5f, 0.5f, 0.5f });

	objSkydome->SetPosition({ -70,0,0 });

	bossBody->SetPosition({ 0,0,20 });
	bossLeg1->SetPosition({ 2,-2,2 });
	bossLeg2->SetPosition({ 2,-2,-2 });
	bossLeg3->SetPosition({ -2,-2,2 });
	bossLeg4->SetPosition({ -2,-2,-2 });

	objC->SetPosition({ 0,0,5 });
	objC->SetRotation({ 0,0,0 });

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

	// ボス関連
	bossHp = 10;
	bossLeg1Hp = 10;
	bossLeg2Hp = 10;
	bossLeg3Hp = 10;
	bossLeg4Hp = 10;

	bossLeg1Break = false;
	bossLeg2Break = false;
	bossLeg3Break = false;
	bossLeg4Break = false;

	bossFlag = true;
	bossLeg1Flag = true;
	bossLeg2Flag = true;
	bossLeg3Flag = true;
	bossLeg4Flag = true;

	// デスフラグ
	bossDeathFlag = false;

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
	//// プレイヤーの座標を表示
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
	debugText.Print(TimeRate.str(), 50, 190, 1.0f);

	std::ostringstream StartIndex;
	StartIndex << "StartIndex:("
		<< std::fixed << std::setprecision(2)
		<< startIndex << ")";
	debugText.Print(StartIndex.str(), 50, 210, 1.0f);*/

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
	debugText.Print("WASD:PlayerMove", 1050, 30, 1.0f);

	debugText.Print("R:CameraReset", 1050, 50, 1.0f);

	debugText.Print("Y:CameraSwitch", 1050, 70, 1.0f);
}
// カメラ方向の切り替え
void GameScreen::CameraSwitching()
{
	if (cameraMode == 0)
	{
		playerPosition.z = 0;

		CameraPos = { centerPosition.x, centerPosition.y, centerPosition.z - 10 };
	}

	else if (cameraMode == 1)
	{
		playerPosition.x = 0;

		CameraPos = { centerPosition.x - 10, centerPosition.y, centerPosition.z };
	}

	else if (cameraMode == 2)
	{
		playerPosition.z = 0;

		CameraPos = { centerPosition.x, centerPosition.y, centerPosition.z + 10 };
	}

	else if (cameraMode == 3)
	{
		playerPosition.x = 0;

		CameraPos = { centerPosition.x + 10, centerPosition.y, centerPosition.z };
	}

	else if (cameraMode >= 4)
	{
		cameraMode = 0;
	}

	if (input->TriggerKey(DIK_Y)) { cameraMode += 1; }
}

void GameScreen::Attack()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet = Bullet::Create(modelBullet, playerBulletPosition, bulletScale, bulletVelocity);

		bullets.push_back(std::move(newBullet));
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