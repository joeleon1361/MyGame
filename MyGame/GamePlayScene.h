#pragma once
#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"

class GamePlayScene
{
public:
	// 初期化
	void Initialize();

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();
};

