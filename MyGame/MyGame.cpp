#include "MyGame.h"

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	FrameWork::Initialize();

	/*titleScene = new Title();
	titleScene->Initialize();*/
}

void MyGame::Finalize()
{
	/*titleScene->Finalize();
	delete titleScene;*/

	//基底クラスの終了処理
	FrameWork::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	FrameWork::Update();

	//titleScene->Update();
}

void MyGame::Draw()
{
	//基底クラスの描画処理
	FrameWork::Draw();
}
