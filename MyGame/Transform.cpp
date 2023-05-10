#include "Transform.h"

using namespace DirectX;

XMFLOAT3 Transform::TransformWorldPosition(XMFLOAT3 localPosition, XMMATRIX matWorld)
{
	XMVECTOR positionV;
	XMFLOAT3 worldPosition;
	positionV = DirectX::XMLoadFloat3(&localPosition);
	positionV.m128_f32[3] = 1.0f;
	positionV = DirectX::XMVector3Transform(positionV, matWorld);
	DirectX::XMStoreFloat3(&worldPosition, positionV);
	return worldPosition;
}