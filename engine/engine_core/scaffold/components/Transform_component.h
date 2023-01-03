#pragma once

#include"core/meta/reflection/reflection.h"
#include"core/math/vector3.h"
#include"core/math/quaternion.h"

#include"engine_core/scaffold/components/component.h"

namespace Mage {
	REFLECTION_TYPE(TransformComponent)
	CLASS(TransformComponent, WhiteListFields) {
		REFLECTION_BODY(TransformComponent)
	public:
	private:
		META(Enable)
		Vector3 position {Vector3::zero};
		META(Enable)
		Quaternion rotation{ Quaternion::identity };
		META(Enable)
		Vector3 scale{ Vector3::one };
	}
}