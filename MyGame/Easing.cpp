#include "Easing.h"

using namespace DirectX;

// Quad(2èÊ)
float Easing::InQuadFloat(const float start, const float end, const float time)
{
	float t = powf(time, 2);
	
	return start * (1.0f - t) + end * t;
}

float Easing::OutQuadFloat(const float start, const float end, const float time)
{
	float t = (1 - powf(1 - time, 2));

	return start * (1.0f - t) + end * t;
}

float Easing::InOutQuadFloat(const float start, const float end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 2 * powf(time, 2);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 2) / 2);
	}

	return start * (1.0f - t) + end * t;
}

XMFLOAT2 Easing::InQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = powf(time, 2);
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::OutQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = (1 - powf(1 - time, 2));
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::InOutQuadFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 2 * powf(time, 2);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 2) / 2);
	}

	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Easing::InQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = powf(time, 2);
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::OutQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = (1 - powf(1 - time, 2));
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::InOutQuadFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 2 * powf(time, 2);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 2) / 2);
	}

	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

// Cubic(3èÊ)
float Easing::InCubicFloat(const float start, const float end, const float time)
{
	float t = powf(time, 3);

	return start * (1.0f - t) + end * t;
}

float Easing::OutCubicFloat(const float start, const float end, const float time)
{
	float t = (1 - powf(1 - time, 3));

	return start * (1.0f - t) + end * t;
}

float Easing::InOutCubicFloat(const float start, const float end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 4 * powf(time, 3);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 3) / 2);
	}

	return start * (1.0f - t) + end * t;
}

XMFLOAT2 Easing::InCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = powf(time, 3);
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::OutCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = (1 - powf(1 - time, 3));
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::InOutCubicFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 4 * powf(time, 3);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 3) / 2);
	}

	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Easing::InCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = powf(time, 3);
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::OutCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = (1 - powf(1 - time, 3));
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::InOutCubicFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 4 * powf(time, 3);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 3) / 2);
	}

	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

// Quart(4èÊ)
float Easing::InQuartFloat(const float start, const float end, const float time)
{
	float t = powf(time, 4);

	return start * (1.0f - t) + end * t;
}

float Easing::OutQuartFloat(const float start, const float end, const float time)
{
	float t = (1 - powf(1 - time, 4));

	return start * (1.0f - t) + end * t;
}

float Easing::InOutQuartFloat(const float start, const float end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 8 * powf(time, 4);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 4) / 2);
	}

	return start * (1.0f - t) + end * t;
}

XMFLOAT2 Easing::InQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = powf(time, 4);
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::OutQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = (1 - powf(1 - time, 4));
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::InOutQuartFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 8 * powf(time, 4);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 4) / 2);
	}

	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Easing::InQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = powf(time, 4);
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::OutQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = (1 - powf(1 - time, 4));
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::InOutQuartFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 8 * powf(time, 4);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 4) / 2);
	}

	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

// Quint(5èÊ)
float Easing::InQuintFloat(const float start, const float end, const float time)
{
	float t = powf(time, 5);

	return start * (1.0f - t) + end * t;
}

float Easing::OutQuintFloat(const float start, const float end, const float time)
{
	float t = (1 - powf(1 - time, 4));

	return start * (1.0f - t) + end * t;
}

float Easing::InOutQuintFloat(const float start, const float end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 16 * powf(time, 5);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 5) / 2);
	}

	return start * (1.0f - t) + end * t;
}

XMFLOAT2 Easing::InQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = powf(time, 5);
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::OutQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t = (1 - powf(1 - time, 5));
	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT2 Easing::InOutQuintFloat2(const XMFLOAT2& start, const XMFLOAT2& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 16 * powf(time, 5);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 5) / 2);
	}

	XMFLOAT2 A, B;
	A = XMFLOAT2(start.x * (1.0f - t), start.y * (1.0f - t));
	B = XMFLOAT2(end.x * t, end.y * t);

	XMFLOAT2 position;
	position = XMFLOAT2(A.x + B.x, A.y + B.y);
	return position;
}

XMFLOAT3 Easing::InQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = powf(time, 5);
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::OutQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t = (1 - powf(1 - time, 5));
	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}

XMFLOAT3 Easing::InOutQuintFloat3(const XMFLOAT3& start, const XMFLOAT3& end, const float time)
{
	float t;
	if (time < 0.5f)
	{
		t = 16 * powf(time, 5);
	}
	else
	{
		t = (1 - powf(-2 * time + 2, 5) / 2);
	}

	XMFLOAT3 A, B;
	A = XMFLOAT3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
	B = XMFLOAT3(end.x * t, end.y * t, end.z * t);

	XMFLOAT3 position;
	position = XMFLOAT3(A.x + B.x, A.y + B.y, A.z + B.z);
	return position;
}