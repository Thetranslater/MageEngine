#include<core/math/math.h>

#include"core/math/vector3.h"

namespace Mage {
	const float Mathf::PI = 3.14159265358979323846264338327950288f;
	const float Mathf::Rad2Deg = 180.f / PI;
	const float Mathf::Episilon = FLT_EPSILON;
	const float Mathf::Deg2Rad = PI / 180.f;
	const float Mathf::Infinity = std::numeric_limits<float>::infinity();
	const float Mathf::NegativeInfinity = -std::numeric_limits<float>::infinity();

	Vector3 Mathf::Max(const Vector3& l, const Vector3& r) {
		return Vector3{ std::max(l.x,r.x), std::max(l.y,r.y),std::max(l.z,r.z) };
	}
	Vector3 Mathf::Min(const Vector3& l, const Vector3& r) {
		return Vector3{ std::min(l.x,r.x), std::min(l.y,r.y),std::min(l.z,r.z) };
	}
}