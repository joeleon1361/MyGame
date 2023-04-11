#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "fbxsdk.h"
#include "PostEffect.h"
#include "FbxLoader.h"
#include "MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	FrameWork* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}