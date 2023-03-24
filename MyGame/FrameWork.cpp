#include "FrameWork.h"

void FrameWork::Run()
{
	// ゲームの初期化
	Initialize();

	while (true) // ゲームループ
	{
		// 更新
		Update();

		// 終了リクエストを受け取ったら抜ける
		if (IsEndRequest())
		{
			break;
		}

		// 描画
		Draw();
	}

	// ゲームの終了
	Finalize();
}

void FrameWork::Initialize()
{
	// ゲームウィンドウの作成
	win = new WinApp();
	win->CreateGameWindow();

	//DirectX初期化処理
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(win->GetInstance(), win->GetHwnd());
	// オーディオの初期化
	sound = Sound::GetInstance();
	sound->Initialize();

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	// ポストエフェクト用テクスチャの読み込み
	// Sprite::LoadTexture(100, L"Resources/white1x1.png");

	// ポストエフェクトの初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	// 3Dオブジェクト静的初期化
	ObjObject::StaticInitialize(dxCommon->GetDevice());

	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon);
}

void FrameWork::Finalize()
{
	// 各種解放
	safe_delete(gameScene);
	safe_delete(sound);
	// safe_delete(input);
	safe_delete(dxCommon);
	delete postEffect;
	FbxLoader::GetInstance()->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();
	safe_delete(win);
}

void FrameWork::Update()
{
	// メッセージ処理
	if (win->ProcessMessage())
	{
		// ゲームループを抜ける
		isEndRequest = true;
		return;
	}

	// 入力関連の毎フレーム処理
	input->Update();
	// ゲームシーンの毎フレーム処理
	gameScene->Update();
}

void FrameWork::Draw()
{
	// レンダーテクスチャへの描画
		// postEffect->PreDrawScene(dxCommon->GetCommandList());
		// gameScene->Draw();
		// postEffect->PostDrawScene(dxCommon->GetCommandList());

	// 描画開始
	dxCommon->PreDraw();

	// ポストエフェクトの描画
	// postEffect->Draw(dxCommon->GetCommandList());

	// ゲームシーンの描画
	gameScene->Draw();
	// 描画終了
	dxCommon->PostDraw();
}
