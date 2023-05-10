#include "VolumeManager.h"

VolumeManager* VolumeManager::GetInstance()
{
	static VolumeManager instance;

	return &instance;
}

void VolumeManager::Initialize()
{
	//�@�}�X�^�[���� = ���݂̃}�X�^�[���� / 100
	masterVolume = masterVolumeNow / 100.0f;

	//�@���� = ���݂�bgm���� / bgm���ʂ̍ő�
	bgmVolumeRatio = bgmVolumeNow / bgmVolumeMax;
	//�@bgm���� = (���� * �}�X�^�[����) / 100
	bgmVolume = (bgmVolumeRatio * masterVolumeNow) / 100.0f;

	//�@���� = ���݂�se���� / se���ʂ̍ő�
	seVolumeRatio = seVolumeNow / seVolumeMax;
	//�@se���� = (���� * �}�X�^�[����) / 100
	seVolume = (seVolumeRatio * masterVolumeNow) / 100.0f;
}
