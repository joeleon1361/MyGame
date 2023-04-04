#include "SceneFactory.h"

#include "GameScene.h"
#include "Title.h"
#include "Result.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//Å@éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;

   if (sceneName == "TITLE")
     {
        newScene = new Title();
    }

    else if (sceneName == "GAMEPLAY")
    {
        newScene = new GameScene();
    }

    else if (sceneName == "RESULT")
   {
       newScene = new Result();
   }

	return newScene;
}
