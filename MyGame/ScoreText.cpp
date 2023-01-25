#include "ScoreText.h"

ScoreText::ScoreText()
{
}

ScoreText::~ScoreText()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

void ScoreText::Initialize(UINT texnumber)
{
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

void ScoreText::Print(const std::string& text, XMFLOAT2 position ,XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f}, float scale = 1.0f)
{
	// 全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ position.x + fontWidth * scale * i, position.y });
		spriteDatas[spriteIndex]->SetColor(color);
		spriteDatas[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		spriteDatas[spriteIndex]->SetAnchorPoint({ 1, 1 });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });

		// 文字を１つ進める
		spriteIndex++;
	}
}

void ScoreText::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}
