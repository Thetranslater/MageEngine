#include"core/math/math.h"
#include"core/math/aabb.h"

namespace Mage {
	AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Vector3& min_corner, const Vector3& max_corner) :min{ min_corner }, max{ max_corner } {}

	void AxisAlignedBoundingBox::merge(const Vector3& point) {
		min = Mathf::Min(min, point);
		max = Mathf::Max(max, point);
	}
}