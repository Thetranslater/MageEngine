#pragma once

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

		void SetPosition(const Vector3& new_p) { position = new_p; position_write_buffer = new_p; }
		void SetRotation(const Quaternion& new_q) { rotation = new_q; rotation_write_buffer = new_q; }
		void SetScale(const Vector3& new_s) { scale = new_s; scale_write_buffer = new_s; }

		void WritePosition(const Vector3& new_p) { position_write_buffer = new_p; }
		void WriteRotation(const Quaternion& new_q) { rotation_write_buffer = new_q; }
		void WriteScale(const Vector3& new_s) { scale_write_buffer = new_s; }

		Matrix4x4 localToWorldMatrix() { return Matrix4x4::TRS(position, rotation, scale); }
		Matrix4x4 worldToLocalMatrix() { Matrix4x4 ret; Matrix4x4::Inverse3DAffine(Matrix4x4::TRS(position, rotation, scale), ret); return ret; }

	protected:
		META(Enable)
		Vector3 position {Vector3::zero};
		META(Enable)
		Quaternion rotation {Quaternion::identity};
		META(Enable)
		Vector3 scale {Vector3::one};

		Vector3 position_write_buffer{ Vector3::zero };
		Quaternion rotation_write_buffer{ Quaternion::identity };
		Vector3 scale_write_buffer{ Vector3::one };
	};
}