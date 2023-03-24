#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sound.h"
#include "GameScene.h"
#include "fbxsdk.h"
#include "PostEffect.h"
#include "FbxLoader.h"

class FrameWork
{
public: // �����o�֐�

	// ���s
	void Run();

	// ������
	virtual void Initialize();

	// �I��
	virtual void Finalize();

	// �X�V
	virtual void Update();

	// �`��
	virtual void Draw();

	// �I���`�F�b�N
	virtual bool IsEndRequest() { return isEndRequest; }
	
	virtual ~FrameWork() = default;

protected: //�����o�ϐ�
//�Q�[�����[�v�I�����N�G�X�g
	bool isEndRequest = false;
};

