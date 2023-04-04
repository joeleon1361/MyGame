#include "Result.h"

Result::Result()
{
}

Result::~Result()
{
}

void Result::Initialize()
{
	// �T�E���h������
	sound->Initialize();
}

void Result::Finalize()
{
}

void Result::Update()
{
}

void Result::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	ObjObject::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	
	// 3D�I�u�W�F�N�g�`��㏈��
	ObjObject::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �`��
	

	// �f�o�b�O�e�L�X�g�̕`��
	// debugText.DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
