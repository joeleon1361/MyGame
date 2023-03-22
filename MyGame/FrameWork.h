#pragma once
class FrameWork
{
public: // メンバ関数

	// 実行
	void Run();

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了チェック
	virtual bool IsEndRequest() { return isEndRequest; }
	
	virtual ~FrameWork() = default;

protected: //メンバ変数
//ゲームループ終了リクエスト
	bool isEndRequest = false;
};

