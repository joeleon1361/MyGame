#include "FrameWork.h"

void FrameWork::Run()
{
	// �Q�[���̏�����
	Initialize();

	while (true) // �Q�[�����[�v
	{
		// �X�V
		Update();

		// �I�����N�G�X�g���󂯎�����甲����
		if (IsEndRequest()) 
		{
			break; 
		}

		// �`��
		Draw();
	}

	// �Q�[���̏I��
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
