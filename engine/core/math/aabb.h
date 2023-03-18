#pragma once

#include<core/meta/reflection/reflection.h>

#include<core/math/vector3.h>

namespace Mage {
	REFLECTION_TYPE(AxisAlignedBoundingBox)
	CLASS(AxisAlignedBoundingBox, Fields) {
		REFLECTION_BODY(AxisAlignedBoundingBox)
public:
	Vector3 min{ FLT_MAX, FLT_MAX,FLT_MAX };
	Vector3 max{ -FLT_MAX,-FLT_MAX,-FLT_MAX };

	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(const Vector3 & min_corner, const Vector3 & max_corner);
	void merge(const Vector3 & point);
	};
}