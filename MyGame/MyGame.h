#pragma once
#include "FrameWork.h"
#include "Title.h"

// ゲーム固有のクラス
class MyGame : public FrameWork
{
public: //メンバ関数
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private: //メンバ変数
	// タイトル
	Title* titleScene = nullptr;
};

