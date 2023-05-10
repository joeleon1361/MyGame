#pragma once
class VolumeManager
{
public: // 静的メンバ関数
	static VolumeManager* GetInstance();

public: // メンバ関数
	void Initialize();

public: // メンバ変数
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

