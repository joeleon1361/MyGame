#pragma once

#include <DirectXMath.h>

class ObjObject3d;
class BaseCollider;


// 衝突情報
struct CollisionInfo
{
public:
	CollisionInfo(ObjObject3d* object, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のオブジェクト
	ObjObject3d* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};

