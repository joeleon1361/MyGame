#include "Spline.h"

using namespace DirectX;

XMFLOAT3 Spline::SplineFloat3(const std::vector<XMFLOAT3>& points, size_t startindex, float time)
{
	size_t n = points.size() - 2;

	if (startindex > n) return points[n];
	if (startindex < 1) return points[1];

	XMFLOAT3 p0 = points[startindex - 1];
	XMFLOAT3 p1 = points[startindex];
	XMFLOAT3 p2 = points[startindex + 1];
	XMFLOAT3 p3 = points[startindex + 2];

	/*XMFLOAT3 position = { 0.5 * (2 * p1 + (-p0 + p2) * time +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * time * time +
		(-p0 + 3 * p1 - 3 * p2 + p3) * time * time * time) };*/

	XMFLOAT3 A, B, C, D, E, F, G, H, I, J;
	A = XMFLOAT3(2 * p1.x, 2 * p1.y, 2 * p1.z);
	B = XMFLOAT3(-p0.x, -p0.y, -p0.z);
	C = XMFLOAT3(p2.x, p2.y, p2.z);
	D = XMFLOAT3(2 * p0.x, 2 * p0.y, 2 * p0.z);
	E = XMFLOAT3(-5 * p1.x, -5 * p1.y, -5 * p1.z);
	F = XMFLOAT3(4 * p2.x, 4 * p2.y, 4 * p2.z);
	G = XMFLOAT3(-p3.x, -p3.y, -p3.z);
	H = XMFLOAT3(3 * p1.x, 3 * p1.y, 3 * p1.z);
	I = XMFLOAT3(-3 * p2.x, -3 * p2.y, -3 * p2.z);
	J = XMFLOAT3(p3.x, p3.y, p3.z);

	/*XMFLOAT3 position = { 0.5 * (A + (B + C) * time +
		(D + E + F + G) * time * time +
		(B + H + I + J) * time * time * time) };*/

	XMFLOAT3 K, L, M;
	K = { A.x + (B.x + C.x) * time, A.y + (B.y + C.y) * time, A.z + (B.z + C.z) * time };
	L = { (D.x + E.x + F.x + G.x) * time * time, (D.y + E.y + F.y + G.y) * time * time, (D.z + E.z + F.z + G.z) * time * time };
	M = { (B.x + H.x + I.x + J.x) * time * time * time, (B.y + H.y + I.y + J.y) * time * time * time, (B.z + H.z + I.z + J.z) * time * time * time };

	XMFLOAT3 N;
	N = { K.x + L.x + M.x, K.y + L.y + M.y ,K.z + L.z + M.z };

	XMFLOAT3 position = { N.x * 0.5f, N.y * 0.5f, N.z * 0.5f };

	return position;
}
