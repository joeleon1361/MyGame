#pragma once

#include "Input.h"

class VolumeManager
{
public: // �ÓI�����o�֐�
	static VolumeManager* GetInstance();

public: // �����o�֐�
	// ������
	void Initialize();
	// ���ʒ���
	void VolumeControl();

private: // �����o�ϐ�
	Input* input = Input::GetInstance();

public: // �����o�ϐ�
	float masterVolumeNow = 7.0f;
	float masterVolume = 0.0f;

	float bgmVolumeMax = 10.0f;
	float bgmVolumeNow = 10.0f;
	float bgmVolumeRatio = 0.0f;
	float bgmVolume = 0.0f;

	float seVolumeMax = 10.0f;
	float seVolumeNow = 10.0f;
	float seVolumeRatio = 0.0f;
	float seVolume = 0.0f;
};

