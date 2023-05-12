#pragma once

#include "Input.h"

class VolumeManager
{
public: // 静的メンバ関数
	static VolumeManager* GetInstance();

public: // メンバ関数
	// 初期化
	void Initialize();
	// 音量調整
	void VolumeControl();

private: // メンバ変数
	Input* input = Input::GetInstance();

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

