#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "GameScene.h"
#include "fbxsdk.h"
#include "PostEffect.h"
#include "FbxLoader.h"

#include "SceneManager.h"
#include "AbstractSceneFactory.h"

class FrameWork
{
public: // メンバ関数

	// 実行
	void Run();

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 終了チェック
	virtual bool IsEndRequest() { return isEndRequest; }
	
	virtual ~FrameWork() = default;

protected: //メンバ変数
//ゲームループ終了リクエスト
	bool isEndRequest = false;

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;
	GameScene* gameScene = nullptr;
	PostEffect* postEffect = nullptr;
	
	AbstractSceneFactory* sceneFactory = nullptr;
};

