#include "SceneFactory.h"

#include "GameScene.h"
#include "Title.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//　次のシーンを生成
	BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new Title();
    }

    else if (sceneName == "GAMEPLAY")
    {
        //newScene = new GameScene();
    }

	return newScene;
}
