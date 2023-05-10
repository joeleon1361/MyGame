#include "VolumeManager.h"

VolumeManager* VolumeManager::GetInstance()
{
	static VolumeManager instance;

	return &instance;
}

void VolumeManager::Initialize()
{
	//　マスター音量 = 現在のマスター音量 / 100
	masterVolume = masterVolumeNow / 100.0f;

	//　割合 = 現在のbgm音量 / bgm音量の最大
	bgmVolumeRatio = bgmVolumeNow / bgmVolumeMax;
	//　bgm音量 = (割合 * マスター音量) / 100
	bgmVolume = (bgmVolumeRatio * masterVolumeNow) / 100.0f;

	//　割合 = 現在のse音量 / se音量の最大
	seVolumeRatio = seVolumeNow / seVolumeMax;
	//　se音量 = (割合 * マスター音量) / 100
	seVolume = (seVolumeRatio * masterVolumeNow) / 100.0f;
}
