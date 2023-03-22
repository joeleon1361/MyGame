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
}

void FrameWork::Finalize()
{
}

void FrameWork::Update()
{
}
