#pragma once

#include "BaseScene.h"

#include <string>

// ƒV[ƒ“Hê(ŠT”O)
class AbstractSceneFactory
{
public:

	virtual ~AbstractSceneFactory() = default;

	// ƒV[ƒ“¶¬
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

};

