#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <cassert>

class SpriteCommon
{
public: // メンバ関数
	// インスタンス取得
	static SpriteCommon* GetInstance();

	// 初期化
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height);

private: //メンバ変数
	// デバイス
	ID3D12Device* device = nullptr;

	//コマンドリスト
	ID3D12GraphicsCommandList* cmdList = nullptr;
};

