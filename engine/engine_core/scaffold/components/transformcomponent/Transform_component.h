#pragma once

#include"core/meta/reflection/reflection.h"
#include"core/math/vector3.h"
#include"core/math/quaternion.h"
#include"core/math/matrix4x4.h"

#include"engine_core/scaffold/components/component.h"

namespace Mage {
	REFLECTION_TYPE(TransformComponent)
	CLASS(TransformComponent:public Component, WhiteListFields) {
		REFLECTION_BODY(TransformComponent)
	public:
		TransformComponent() { should_tick_in_editor = true; }
		void tick(float delta) override;

		Vector3 Position() { return position; }
		Quaternion Rotation() { return rotation; }
		Vector3 Scale() { return scale; }

		void SetPosition(const Vector3 & new_p) { position = new_p; }
		void SetRotation(const Quaternion & new_q) { rotation = new_q; }
		void SetScale(const Vector3 & new_s) { scale = new_s; }

		Matrix4x4 localToWorldMatrix() { return Matrix4x4::TRS(position, rotation, scale); }
		Matrix4x4 worldToLocalMatrix() { Matrix4x4 ret; Matrix4x4::Inverse3DAffine(Matrix4x4::TRS(position, rotation, scale), ret); return ret; }

	protected:
		META(Enable)
		Vector3 position {Vector3::zero};
		META(Enable)
		Quaternion rotation {Quaternion::identity};
		META(Enable)
		Vector3 scale {Vector3::one};
	};
}