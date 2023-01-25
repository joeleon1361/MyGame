#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

class ScoreText
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256;	// 最大文字数
	static const int fontWidth = 32;			// フォント画像内1文字分の横幅
	static const int fontHeight = 36;		// フォント画像内1文字分の縦幅
	static const int fontLineCount = 11;	// フォント画像内1行分の文字数

	ScoreText();
	~ScoreText();

	void Initialize(UINT texnumber);

	void Print(const std::string& text, XMFLOAT2 position, XMFLOAT4 color, float size);

	void DrawAll(ID3D12GraphicsCommandList* cmdList);

private:
	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;
};

