#include "MyGame.h"
#include "SceneFactory.h"

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	FrameWork::Initialize();

	//シーンの初期化処理
	//シーンファクトリを生成し、マネージャーにセット
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	//シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	//基底クラスの終了処理
	FrameWork::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	FrameWork::Update();
}

void MyGame::Draw()
{
	//基底クラスの描画処理
	FrameWork::Draw();
}
