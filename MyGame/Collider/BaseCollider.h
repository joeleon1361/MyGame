#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ObjObject.h"

// コライダー基底クラス
class BaseCollider
{
public:
	BaseCollider() = default;
	// 仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject(ObjObject* object) {
		this->object3d = object;
	}

	inline ObjObject* GetObject3d() {
		return object3d;
	}

	// 更新
	virtual void Update() = 0;
	// 形状タイプ
	inline CollisionShapeType GetShapeType() { return shapeType; }
	
	// 衝突時コールバック関数
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

protected:
	ObjObject* object3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};

