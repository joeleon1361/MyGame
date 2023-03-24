#pragma once

#include "SafeDelete.h"

#include "BaseScene.h"
#include "DirectXCommon.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

#include "Input.h"
#include "Sprite.h"
#include "ObjObject.h"
#include "Sound.h"
#include "Camera.h"

class Title : public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // �ÓI�����o�ϐ�
	// �X�v���C�g�̃e�N�X�`���ԍ�
	static enum TextureNumber
	{
		// �^�C�g��
		title_gtxt_1,
		title_parts_1,
		loading_effect_1
	};

public:
	// ������
	void Initialize();

	// �I��
	void Finalize();

	// �X�V
	void Update();

	// �`��
	void Draw();

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Sound* sound = nullptr;

	// �Q�[���V�[���p
	Camera* camera = nullptr;

	Sprite* TitleStartUI = nullptr;
	Sprite* TitleLogo = nullptr;
	Sprite* LoadingBG = nullptr;

	ObjObject* objTitlePlayer = nullptr;
	ObjObject* objSkydome = nullptr;

	ObjModel* modelPlayer = nullptr;
	ObjModel* modelSkydome = nullptr;

	// �v���C���[�֘A
	XMFLOAT3 TitlePlayerPosition;
	XMFLOAT3 TitlePlayerRotation;

	XMFLOAT4 titleStartUIColor;

	// �X�J�C�h�[���̍��W
	XMFLOAT3 SkydomPosition;
	// �X�J�C�h�[���̊p�x
	XMFLOAT3 SkydomRotation;

	int titleScene = WAITING;

	float backTimer = 40.0f;
	float stagingTimer = 60.0f;

	bool stagingFlag = false;

	float moveY = 0.01f;
	float moveX = 0.05f;

	bool addSpeedX = false;
	bool subSpeedX = false;

	bool addSpeedY = false;
	bool subSpeedY = false;

	bool addAngleX = false;
	bool subAngleX = false;

	// ���[�f�B���O�̈Ó]�F
	XMFLOAT4 loadingColor;

	// �V�[���J�ڎ��̈Ó]�A���t�@�l
	float loadingChangeAlpha = 0.0f;

	bool changeColorFlag = false;
	float changeColorTimer = 30.0f;

	bool changeSceneFlag = false;
	float changeSceneTimer = 100.0f;

	enum TITLESCENE
	{
		WAITING,
		STAGING,
		MOVESCENE
	};
};

