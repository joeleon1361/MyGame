#include "Title.h"

using namespace DirectX;

Title::Title()
{
}

Title::~Title()
{
}

void Title::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	ObjObject::SetCamera(camera);

	LoadTextureFunction();

	// スプライト生成
	TitleLogo = Sprite::Create(TextureNumber::title_gtxt_1, { 100.0f,100.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	TitleStartUI = Sprite::Create(TextureNumber::title_parts_1, { 310.0f,630.0f });
	LoadingBG = Sprite::Create(TextureNumber::loading_effect_1, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f });

	playerJetParticle = ParticleManager::Create(dxCommon->GetDevice(), camera, 1, L"Resources/effect1.png");

	// 3Dオブジェクト生成
	objTitlePlayer = ObjObject::Create();
	modelPlayer = ObjModel::CreateFromOBJ("playerBody");
	objTitlePlayer->SetModel(modelPlayer);

	objTitlePlayerTurret = ObjObject::Create();
	modelPlayerTurret = ObjModel::CreateFromOBJ("playerTurret");
	objTitlePlayerTurret->SetModel(modelPlayerTurret);

	objSkydome = ObjObject::Create();
	modelSkydome = ObjModel::CreateFromOBJ("skydome");
	objSkydome->SetModel(modelSkydome);

	sound->LoadWav("BGM/Title/title_bgm.wav");
	sound->LoadWav("SE/Title/title_start.wav");

	objTitlePlayer->SetPosition({ -4.0f,-2.0f,0 });
	objTitlePlayer->SetRotation({ 0, 90, 0 });
	objTitlePlayer->SetScale({ 0.8f, 0.8f, 0.8f });

	objTitlePlayerTurret->SetParent(objTitlePlayer);

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

	volumeManager->Initialize();

	Sound::GetInstance()->PlayWav("BGM/Title/title_bgm.wav", volumeManager->bgmVolume, true);
}

void Title::Finalize()
{
	safe_delete(TitleLogo);
	safe_delete(TitleStartUI);

	safe_delete(objTitlePlayer);
	safe_delete(modelPlayer);
}

void Title::Update()
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
	CreatePlayerJetParticles(TitlePlayerPosition);

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
		if (input->TriggerKey(DIK_SPACE) || input->TriggerKey(DIK_Z) || input->TriggerKey(DIK_A) || input->TriggerKey(DIK_S) || input->TriggerKey(DIK_D))
		{
			sound->PlayWav("SE/Title/title_start.wav", volumeManager->seVolume);
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
			sound->StopWav("BGM/Title/title_bgm.wav");
			//シーン切り替え
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
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

}

void Title::Draw()
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

void Title::LoadTextureFunction()
{
	// タイトル
	if (!Sprite::LoadTexture(TextureNumber::title_gtxt_1, L"Resources/Sprite/TitleUI/title_gtxt_1.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(TextureNumber::title_parts_1, L"Resources/Sprite/TitleUI/title_parts_2.png")) {
		assert(0);
		return;
	}

	// エフェクト
	if (!Sprite::LoadTexture(TextureNumber::loading_effect_1, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}
}

void Title::CreatePlayerJetParticles(XMFLOAT3 position)
{
	for (int i = 0; i < 10; i++) {
		const float rnd_pos = 0.1f;
		XMFLOAT3 pos{};
		pos.x = position.x + -1.5f;
		pos.y = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.y - 0.1f;
		pos.z = ((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + position.z;

		const float rnd_vel = -0.2f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = -0.1f;
		acc.x = -0.1f;

		// 追加
		playerJetParticle->Add(5, pos, vel, acc, { 0.874f,0.443f, 0.149f, 1.000f }, { 0.874f,0.443f, 0.149f, 1.000f }, 0.3f, 0.0f);
	}
}
