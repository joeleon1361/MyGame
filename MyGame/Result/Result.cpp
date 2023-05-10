#include "Result.h"

Result::Result()
{
}

Result::~Result()
{
}

void Result::Initialize()
{
	// サウンド初期化
	sound->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	LoadTextureFunction();

	// スコアテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(common_dtxt_2, L"Resources/Sprite/Common/common_dtxt_2.png")) {
		assert(0);
		return;
	}
	// スコアテキスト初期化
	scoreText.Initialize(common_dtxt_2);

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
	LoadingBG = Sprite::Create(TextureNumber::loading_effect_1, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,0.0f });

	sound->LoadWav("SE/Result/result_open_1.wav");
	sound->LoadWav("SE/Result/result_open_2.wav");
	sound->LoadWav("SE/Result/result_mission.wav");
	sound->LoadWav("SE/Result/result_rank.wav");

	camera->SetTarget({ 0, 0, 0 });
	camera->SetEye({ 0, 0, 10 });
	camera->SetUp({ 0, 1, 0 });

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

void Result::Finalize()
{
}

void Result::Update()
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
	if (scoreManager->noDamageFlag == false)
	{
		noDamageBonus = 0;
	}

	totalScore = scoreManager->score + noDamageBonus;

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
		sound->PlayWav("SE/Result/result_open_1.wav", 0.07f);
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
		sound->PlayWav("SE/Result/result_open_2.wav", 0.07f);

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

		sound->PlayWav("SE/Result/result_rank.wav", 0.07f);

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
		if (scoreManager->noDamageFlag == true)
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
		if (scoreManager->allLegBreakFlag == true)
		{
			resultChangeAlpha_10 = 0.05f;
		}
		if (scoreManager->noDamageFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", volumeManager->seVolume);
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
		if (scoreManager->targetScoreFlag == true)
		{
			resultChangeAlpha_11 = 0.05f;
		}

		if (scoreManager->allLegBreakFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", volumeManager->seVolume);

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
		if (scoreManager->targetScoreFlag == true)
		{
			Sound::GetInstance()->PlayWav("SE/Result/result_mission.wav", volumeManager->seVolume);
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
		if (Input::GetInstance()->TriggerKey(DIK_SPACE) || input->TriggerKey(DIK_Z))
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
		SceneManager::GetInstance()->ChangeScene("TITLE");
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

	// スコアの描画
	std::ostringstream GameScore;
	GameScore << std::fixed << std::setprecision(0) << std::setw(7) << scoreManager->score;
	scoreText.Print(GameScore.str(), scorePosition, { 0.760f, 0.929f, 1.0f, scoreColor.w }, 0.5f);

	std::ostringstream NoDamageBonus;
	NoDamageBonus << std::fixed << std::setprecision(0) << std::setw(7) << noDamageBonus;
	scoreText.Print(NoDamageBonus.str(), noDamageBonusPosition, { 1.0f, 1.0f, 0.4f, noDamageBonusColor.w }, 0.5f);

	std::ostringstream TotalScone;
	TotalScone << std::fixed << std::setprecision(0) << std::setw(7) << totalScore;
	scoreText.Print(TotalScone.str(), totalScorePosition, { 0.760f, 0.929f, 1.0f, totalScoreColor.w }, 0.8f);
}

void Result::Draw()
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


	if (scoreManager->score > 99999)
	{
		// S
		resultGTXT_6->Draw();
	}
	else if (scoreManager->score > 49999)
	{
		// A
		resultGTXT_7->Draw();
	}
	else if (scoreManager->score > 24999)
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

void Result::LoadTextureFunction()
{
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

	// エフェクト
	if (!Sprite::LoadTexture(TextureNumber::loading_effect_1, L"Resources/Sprite/Effect/loading_effect_1.png")) {
		assert(0);
		return;
	}
}
