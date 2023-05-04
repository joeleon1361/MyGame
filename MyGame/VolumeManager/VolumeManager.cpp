#include "VolumeManager.h"

VolumeManager* VolumeManager::GetInstance()
{
	static VolumeManager instance;

	return &instance;
}
