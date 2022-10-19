#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "ObjMesh.h"


// モデルデータ
class ObjModel
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const std::string baseDirectory;

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

public: // 静的メンバ関数

	
	// 静的初期化
	static void StaticInitialize(ID3D12Device* device);

	// OBJファイルからメッシュ生成
	static ObjModel* CreateFromOBJ(const std::string& modelname);

public: // メンバ関数
	
	// デストラクタ
	~ObjModel();
	
	// 初期化
	void Initialize(const std::string& modelname);
	
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

private: // メンバ変数
	// 名前
	std::string name;
	// メッシュコンテナ
	std::vector<ObjMesh*> meshes;
	// マテリアルコンテナ
	std::unordered_map<std::string, ObjMaterial*> materials;
	// デフォルトマテリアル
	ObjMaterial* defaultMaterial = nullptr;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;

private: // メンバ関数
	
	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string & filename);

	// マテリアル登録
	void AddMaterial(ObjMaterial* material);

	// デスクリプタヒープの生成
	void CreateDescriptorHeap();
	
	// テクスチャ読み込み
	void LoadTextures();

};

