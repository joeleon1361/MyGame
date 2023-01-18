#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class ScoreText
{
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 256;	// �ő啶����
	static const int fontWidth = 32;			// �t�H���g�摜��1�������̉���
	static const int fontHeight = 36;		// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 11;	// �t�H���g�摜��1�s���̕�����

	ScoreText();
	~ScoreText();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, float x, float y, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;
};

