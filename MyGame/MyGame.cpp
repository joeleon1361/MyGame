#include "MyGame.h"

void MyGame::Initialize()
{
	//���N���X�̏���������
	FrameWork::Initialize();

	/*titleScene = new Title();
	titleScene->Initialize();*/
}

void MyGame::Finalize()
{
	/*titleScene->Finalize();
	delete titleScene;*/

	//���N���X�̏I������
	FrameWork::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	FrameWork::Update();

	//titleScene->Update();
}

void MyGame::Draw()
{
	//���N���X�̕`�揈��
	FrameWork::Draw();
}
