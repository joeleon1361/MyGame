#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "SceneManager.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include <sstream>
#include <iomanip>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"
#include "ScoreText.h"

class Result : public BaseScene
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
		// 共通
		common_dtxt_2,

		// リザルト
		result_bg,
		result_frame_1,
		result_frame_2,
		result_frame_3,
		result_gtxt_1,
		result_gtxt_2,
		result_gtxt_3,
		result_gtxt_4,
		result_gtxt_5,
		result_gtxt_6,
		result_gtxt_7,
		result_gtxt_8,
		result_gtxt_9,
		result_gtxt_12,
		result_gtxt_13,
		result_gtxt_14,
		result_gtxt_15,
		result_parts_1,
		result_parts_2,
		result_parts_3,
		result_parts_4,
		result_parts_5,
		result_parts_6,

		// エフェクト
		loading_effect_1
	};

public:
	// コンストラクタ
	Result();

	// デストラクタ
	~Result();

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// テクスチャのロード
	void LoadTextureFunction();

private: // メンバ変数
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Input* input = Input::GetInstance();
	Sound* sound = Sound::GetInstance();

	ScoreText scoreText;

	// ゲームシーン用
	Camera* camera = nullptr;

	Sprite* LoadingBG = nullptr;

	Sprite* ResultBG = nullptr;
	Sprite* ResultBN_1 = nullptr;
	Sprite* ResultBN_2 = nullptr;
	Sprite* ResultBN_3 = nullptr;
	Sprite* resultGTXT_1 = nullptr;
	Sprite* resultGTXT_2 = nullptr;
	Sprite* resultGTXT_3 = nullptr;
	Sprite* resultGTXT_4 = nullptr;
	Sprite* resultGTXT_5 = nullptr;
	Sprite* resultGTXT_6 = nullptr;
	Sprite* resultGTXT_7 = nullptr;
	Sprite* resultGTXT_8 = nullptr;
	Sprite* resultGTXT_9 = nullptr;
	Sprite* resultGTXT_12 = nullptr;
	Sprite* resultGTXT_13 = nullptr;
	Sprite* resultGTXT_14 = nullptr;
	Sprite* resultGTXT_15 = nullptr;

	Sprite* resultParts_1 = nullptr;
	Sprite* resultParts_2 = nullptr;
	Sprite* resultParts_3 = nullptr;
	Sprite* resultParts_4 = nullptr;
	Sprite* resultParts_5 = nullptr;
	Sprite* resultParts_6 = nullptr;
	Sprite* resultParts_7 = nullptr;
	Sprite* resultParts_8 = nullptr;
	Sprite* resultParts_9 = nullptr;
	Sprite* resultParts_10 = nullptr;

	// バナー1のサイズ
	XMFLOAT2 resultBN_1Size = {};
	// バナー2のサイズ
	XMFLOAT2 resultBN_2Size = {};
	// バナー3のサイズ
	XMFLOAT2 resultBN_3Size = {};


	XMFLOAT2 resultParts_1Size = {};
	XMFLOAT2 resultParts_2Size = {};
	XMFLOAT2 resultParts_3Size = {};

	// Sランクマークのサイズ
	XMFLOAT2 rankSSize = {};
	// Aランクマークのサイズ
	XMFLOAT2 rankASize = {};
	// Bランクマークのサイズ
	XMFLOAT2 rankBSize = {};
	// Cランクマークのサイズ
	XMFLOAT2 rankCSize = {};

	XMFLOAT2 resultGTXT_15Size = {};

	// ミッション達成スター1のサイズ
	XMFLOAT2 missionStar1Size = {};
	// ミッション達成スター2のサイズ
	XMFLOAT2 missionStar2Size = {};
	// ミッション達成スター3のサイズ
	XMFLOAT2 missionStar3Size = {};

	// スコアテキストの表示座標
	XMFLOAT2 scorePosition = { 855.0f, 275.0f };
	// ノーダメージボーナステキストの表示座標
	XMFLOAT2 noDamageBonusPosition = { 855.0f, 305.0f };
	// 合計スコアテキストの表示座標
	XMFLOAT2 totalScorePosition = { 800.0f, 385.0f };
	// クリアランクテキストの表示座標
	XMFLOAT2 rankPosition = { 800.0f, 455.0f };
	// クリアランクマークの表示座標
	XMFLOAT2 resultRankPosition = { 850.0f,480.0f };

	// リザルトテキストの色
	XMFLOAT4 resultColor = {};
	XMFLOAT4 resultPartsColor = {};
	// スコアテキストの色
	XMFLOAT4 scoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// ノーダメージテキストの色
	XMFLOAT4 noDamageBonusColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// 合計スコアテキストの色
	XMFLOAT4 totalScoreColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// クリアランクテキストの色
	XMFLOAT4 rankColor = { 0.760f, 0.929f, 1.0f, 0.0f };
	// ミッションテキストの色
	XMFLOAT4 missionColor = { 0.760f, 0.929f, 1.0f, 0.0f };

	// Sランクマークの色
	XMFLOAT4 rankSColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Aランクマークの色
	XMFLOAT4 rankAColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Bランクマークの色
	XMFLOAT4 rankBColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Cランクマークの色
	XMFLOAT4 rankCColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	// ミッション1テキストの色
	XMFLOAT4 mission1Color = {};
	// ミッション2テキストの色
	XMFLOAT4 mission2Color = {};
	// ミッション3テキストの色
	XMFLOAT4 mission3Color = {};

	// ミッション未達成スター1の色
	XMFLOAT4 missionStar1Color = {};
	// ミッション未達成スター2の色
	XMFLOAT4 missionStar2Color = {};
	// ミッション未達成スター3の色
	XMFLOAT4 missionStar3Color = {};

	// ミッション達成スター1の色
	XMFLOAT4 missionStar4Color = {};
	// ミッション達成スター2の色
	XMFLOAT4 missionStar5Color = {};
	// ミッション達成スター3の色
	XMFLOAT4 missionStar6Color = {};

	float resultMoveVelX_1 = 0.0f;
	float resultMoveAccX_1 = 0.0f;

	float resultMoveVelY_1 = 0.0f;
	float resultMoveAccY_1 = 0.0f;

	float resultMoveVelX_2 = 0.0f;
	float resultMoveAccX_2 = 0.0f;

	float resultMoveVelX_3 = 0.0f;
	float resultMoveAccX_3 = 0.0f;

	float resultMoveVelX_4 = 0.0f;
	float resultMoveAccX_4 = 0.0f;

	float resultMoveVelX_5 = 0.0f;
	float resultMoveAccX_5 = 0.0f;

	float resultMoveVelX_6 = 0.0f;
	float resultMoveAccX_6 = 0.0f;

	float resultMoveVel_7 = 0.0f;

	float resultMoveVel_8 = 0.0f;

	float resultMoveVel_9 = 0.0f;
	float resultMoveVel_10 = 0.0f;
	float resultMoveVel_11 = 0.0f;

	float resultChangeAlpha_1 = 0.0f;
	float resultChangeAlpha_2 = 0.0f;
	float resultChangeAlpha_3 = 0.0f;
	float resultChangeAlpha_4 = 0.0f;
	float resultChangeAlpha_5 = 0.0f;
	float resultChangeAlpha_6 = 0.0f;
	float resultChangeAlpha_7 = 0.0f;
	float resultChangeAlpha_8 = 0.0f;
	float resultChangeAlpha_9 = 0.0f;
	float resultChangeAlpha_10 = 0.0f;
	float resultChangeAlpha_11 = 0.0f;

	// ローディングの暗転色
	XMFLOAT4 loadingColor;

	// シーン遷移時の暗転アルファ値
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

	// リザルト画面に入ってからの時間を計算するタイマー
	float resultTimer = 5.0f;

	// キー入力が可能かフラグ
	bool canPushKeyFlag = false;

	// リザルトテキストのサイズ計算を開始するフラグ
	bool changeResultSizeFlag = false;

	// ノーダメージ時に加算加算される値
	float noDamageBonus = 20000.0f;

	// 合計スコアの値
	float totalScore = 0.0f;
};

