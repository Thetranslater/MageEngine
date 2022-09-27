#include<core/math/vector3.h>

namespace Mage {
	const Vector3 Vector3::zero{ 0.f,0.f,0.f };
	const Vector3 Vector3::one{ 1.f,1.f,1.f };
	const Vector3 Vector3::forward{ 0.f,0.f,1.f };
	const Vector3 Vector3::back{ 0.f,0.f,-1.f };
	const Vector3 Vector3::right{ 1.f,0.f,0.f };
	const Vector3 Vector3::left{ -1.f,0.f,0.f };
	const Vector3 Vector3::up{ 0.f,1.f,0.f };
	const Vector3 Vector3::down{ 0.f,-1.f,0.f };
}