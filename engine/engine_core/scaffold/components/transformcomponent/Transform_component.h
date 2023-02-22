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
		TransformComponent() :parent{ nullptr } { should_tick_in_editor = true; }

		void tick(float delta) override;

		//std::shared_ptr<Widget> InspectUI() override;

		inline Vector3 Position() { return position; }
		inline Quaternion Rotation() { return rotation; }
		inline Vector3 Scale() { return scale; }

		//attention: Because SetXXX function directly sets the properties of the transform, it may cause inconsistency.
		// Please use WriteXXX when you have the consistency requirement.
		inline void SetPosition(const Vector3& new_p) { position = new_p; position_write_buffer = new_p; }
		inline void SetRotation(const Quaternion& new_q) { rotation = new_q; rotation_write_buffer = new_q; }
		inline void SetScale(const Vector3& new_s) { scale = new_s; scale_write_buffer = new_s; }

		inline void WritePosition(const Vector3& new_p) { position_write_buffer = new_p; }
		inline void WriteRotation(const Quaternion& new_q) { rotation_write_buffer = new_q; }
		inline void WriteScale(const Vector3& new_s) { scale_write_buffer = new_s; }

		inline Matrix4x4 localToWorldMatrix() { return Matrix4x4::TRS(position, rotation, scale); }
		inline Matrix4x4 worldToLocalMatrix() { Matrix4x4 ret; Matrix4x4::Inverse3DAffine(Matrix4x4::TRS(position, rotation, scale), ret); return ret; }

		inline TransformComponent* GetParent() { return parent; }
		inline void SetParent(TransformComponent* p_transform) { parent = p_transform; }

		inline TransformComponent* GetChild(int index) { return children.at(index); }
		inline std::vector<TransformComponent*>& GetChildren() { return children; }

		inline bool IsChildOf() { return parent != nullptr; }
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

		//parent
		TransformComponent* parent;
		std::vector<TransformComponent*> children;
	};
}