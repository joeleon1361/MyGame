#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"

class Title : public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	// スプライトのテクスチャ番号
	static enum TextureNumber
	{
		// タイトル
		title_gtxt_1,
		title_parts_1,
		loading_effect_1
	};

public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* TitleStartUI = nullptr;
	Sprite* TitleLogo = nullptr;
	Sprite* LoadingBG = nullptr;

	ObjObject* objTitlePlayer = nullptr;
	ObjObject* objSkydome = nullptr;

	ObjModel* modelPlayer = nullptr;
	ObjModel* modelSkydome = nullptr;

	// プレイヤー関連
	XMFLOAT3 TitlePlayerPosition;
	XMFLOAT3 TitlePlayerRotation;

	XMFLOAT4 titleStartUIColor;

	// スカイドームの座標
	XMFLOAT3 SkydomPosition;
	// スカイドームの角度
	XMFLOAT3 SkydomRotation;

	int titleScene = WAITING;

	float backTimer = 40.0f;
	float stagingTimer = 60.0f;

	bool stagingFlag = false;

	float moveY = 0.01f;
	float moveX = 0.05f;

	bool addSpeedX = false;
	bool subSpeedX = false;

	bool addSpeedY = false;
	bool subSpeedY = false;

	bool addAngleX = false;
	bool subAngleX = false;

	// ローディングの暗転色
	XMFLOAT4 loadingColor;

	// シーン遷移時の暗転アルファ値
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

	enum TITLESCENE
	{
		WAITING,
		STAGING,
		MOVESCENE
	};
};

