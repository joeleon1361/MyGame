#pragma once

#include "BaseScene.h"
#include "AbstractSceneFactory.h"

// シーン管理
class SceneManager
{
public:
	static SceneManager* GetInstance();

	
	// 更新
	void Update();

	// 描画
	void Draw();

	// 次シーン予約
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory = sceneFactory; }

	//解放
	void Finalize();

private: //メンバ変数
//今のシーン
	BaseScene* scene = nullptr;
	//次のシーン
	BaseScene* nextScene = nullptr;
	//シーン工場
	AbstractSceneFactory* sceneFactory = nullptr;
};

