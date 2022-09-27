#include<core/math/math.h>

namespace Mage {
	const float Mathf::PI = 3.14159265358979323846264338327950288f;
	const float Mathf::Rad2Deg = 180.f / PI;
	const float Mathf::Episilon = FLT_EPSILON;
	const float Mathf::Deg2Rad = PI / 180.f;
	const float Mathf::Infinity = std::numeric_limits<float>::infinity();
	const float Mathf::NegativeInfinity = -std::numeric_limits<float>::infinity();
}