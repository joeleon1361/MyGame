#include "SpriteCommon.h"

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;

	return &instance;
}

void SpriteCommon::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	this->device = device;
	this->cmdList = cmdList;
}
