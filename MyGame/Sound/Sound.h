#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

#include <wrl.h>
#include <map>
#include <string>

// オーディオ
class Sound
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public: // サブクラス
	// チャンクヘッダ
	struct ChunkHeader
	{
		char	id[4]; // チャンク毎のID
		int32_t		size;  // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader	chunk;   // "RIFF"
		char	type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader		chunk; // "fmt "
		WAVEFORMATEX	fmt;   // 波形フォーマット
	};

	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;

		//バッファの先頭アドレス
		BYTE* pBuffer;

		//バッファのサイズ
		unsigned int bufferSize;

		IXAudio2SourceVoice* pSourceVoice;
	};

public: // 静的メンバ関数
	static Sound* GetInstance();

public: // メンバ関数
	
	// 初期化
	void Initialize(const std::string& directoryPath = "Resources/Sound/");

	// 解放
	void Finalize();

	// 音声読み込み
	void LoadWav(const std::string& filename);

	// サウンドデータの解放
	void UnLoad(SoundData* soundData);

	// サウンドファイルの読み込みと再生
	void PlayWav(const std::string& filename, const float Volume, bool Loop = false);

	// サウンドファイルの停止
	void StopWav(const std::string& filename);

private: // メンバ変数
	ComPtr<IXAudio2> xAudio2;

	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas;

	//サウンド格納ディレクトリ
	std::string directoryPath_;

	XAUDIO2_BUFFER buf{};
};

