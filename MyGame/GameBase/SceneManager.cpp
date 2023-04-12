#include "SceneManager.h"

#include <cassert>

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (nextScene)
	{
		//旧シーンの終了
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}

		//シーン切り替え
		scene = nextScene;
		nextScene = nullptr;

		//新シーンの初期化
		scene->Initialize(/*dxCommon*/);

	}

	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//次シーンを生成
	nextScene = sceneFactory->CreateScene(sceneName);
}

void SceneManager::Finalize()
{
	//最後のシーンの終了と解放
	scene->Finalize();
	delete scene;
}
