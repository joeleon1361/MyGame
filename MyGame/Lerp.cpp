#include "Lerp.h"

using namespace DirectX;

XMFLOAT2 Lerp::LerpFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - time), start.y * (1.0f - time));
	B = XMFLOAT2(end.x * time, end.y * time);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Lerp::LerpFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - time), start.y * (1.0f - time), start.z * (1.0f - time));
	B = XMFLOAT3(end.x * time, end.y * time, end.z * time);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}